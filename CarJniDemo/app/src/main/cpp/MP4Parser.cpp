//
// Created by admin on 2017/11/20.
//

#include "MP4Parser.h"

#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <cstdlib>
#include <cerrno>
static union ____indian_swap{
    unsigned char byte[4];
    unsigned int size;
}indian_a , indian_b;

unsigned read_uint16_big(FILE *f)
{
    unsigned short k = 0;
    fread(&k, sizeof(k), 1, f);
    return k;
}
uint16_t read_uint16_lit(FILE *f)
{
    uint16_t k;
    fread(&k, sizeof(k), 1, f);
    return ((k&0xff00) >> 8)|((k&0xff) << 8);
}

float read_float_big(FILE *f)
{
    float val = 0.0;
    fread(&val,sizeof(val),1,f);
    return val;
}

unsigned char read_uint8(FILE *f)
{
    unsigned char x;
    fread(&x, sizeof(x), 1, f);
    return x;
}

int read_uint32_lit(FILE *f){
    int k = 0;
    fread(&k, sizeof(k), 1, f);
    indian_a.size = k;
    for(int i = 0, j = 3; i < 4; i++, j--)
        indian_b.byte[i] = indian_a.byte[j];

    return indian_b.size;
}

unsigned read_uint32_big(FILE *f)
{
    unsigned int k = 0;
    fread(&k, sizeof(k), 1, f);
    return k;
}

void mp4_read_gps_data(FILE* f,int count,int *gpsa,RMCINFO *infos)
{
    RMCINFO gps;
    for(int i=0;i<count;i++)
    {
        fseek(f, gpsa[i]+48, SEEK_SET);
        gps.Hour=read_uint32_big(f);
        gps.Minute=read_uint32_big(f);
        gps.Second=read_uint32_big(f);
        gps.Year=read_uint32_big(f);
        gps.Month=read_uint32_big(f);
        gps.Day=read_uint32_big(f);

        gps.Status = read_uint8(f);
        gps.NSInd = read_uint8(f);
        gps.EWInd = read_uint8(f);
        gps.reservd = read_uint8(f);
        gps.Latitude=read_float_big(f);
        gps.Longitude=read_float_big(f);
        gps.Speed=read_float_big(f);
        gps.Angle=read_float_big(f);
        *(infos+i) = gps;
    }
    free(gpsa);
}


void mp4_read_gps_box(FILE* f, int size,int *timeSpan,GPSResult *result)
{

    int ver  = 0;
    int count  = 0;
    long cur_pos           = ftell(f);
    //版本信息
    fseek(f, cur_pos, SEEK_SET);
    ver  = read_uint32_lit(f);
    count  = read_uint32_lit(f);
    result->info = (RMCINFO *)malloc(count * sizeof(RMCINFO));
    *timeSpan = count;
    int *gpsa = (int *)malloc(count * sizeof(int));
    for (int k=0; k<count; k++) {
        gpsa[k]=read_uint32_lit(f);
        read_uint32_lit(f);
    }
    mp4_read_gps_data(f,count,gpsa,result->info);
}


int mp4_read_moov_box(FILE* f, int size,int *timeSpan,GPSResult *result)   //level 2
{
    int k = 0;
    unsigned char p[5];
    int inner_size = 0;
    int level_2_box_size  = 0;
    long cur_pos           = ftell(f);
    do{
        fseek(f, cur_pos, SEEK_SET);
        level_2_box_size  = read_uint32_lit(f);
        fread(&k, sizeof(k), 1, f);  //read byte
        indian_a.size     = k;
        memcpy(p, indian_a.byte, sizeof(indian_a.byte));
        p[4]              = 0;
        std::string name  = (char*)p;
        if(name == "gps "){
            mp4_read_gps_box(f,level_2_box_size,timeSpan,result);
        }
        cur_pos    += level_2_box_size;
        inner_size += level_2_box_size;
    }while( inner_size+8 != size);
    return 0;
}

int mp4_read_root_box(FILE *f ,int isloadgps,int *timeSpan,GPSResult *result) //level 1
{
    int k = 0;
    unsigned char p[5];
    int level_1_box_size  = read_uint32_lit(f);
    fread(&k, sizeof(k), 1, f);  //read byte
    indian_a.size         = k;
    memcpy(p, indian_a.byte, sizeof(indian_a.byte));
    p[4]                  = 0;
    std::string name      = (char*)p;
    if(name == "moov"){
        if( isloadgps ==0)
        {
            mp4_read_moov_box(f, level_1_box_size,timeSpan,result);
        }
    }
    else if(level_1_box_size == 0){  //till the end of file
        return 1;
    }
    return level_1_box_size;
}

GPSResult* parseMP4(const char *filePath) {
    GPSResult *result = (GPSResult *)malloc(1 * sizeof(GPSResult));
    result->timeSpan = 0;
    result->feedback = 0;
    int tmp = 0;
    if (filePath == NULL) {
        result->feedback = 1;
        return result;
    }
    std::ios_base::sync_with_stdio(true);
    FILE *fin  = fopen(filePath, "r");
    if(fin == (FILE*)0){
        result->feedback = 1;
        return result;
    }
    long cur_pos = ftell(fin);
    for(; !feof(fin); ) {
        fseek(fin, cur_pos, SEEK_SET);
        cur_pos += mp4_read_root_box(fin,0,&tmp,result);
    }
    result->timeSpan = tmp;
    return result;
}