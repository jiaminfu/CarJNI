//
// Created by admin on 2017/11/20.
//

#ifndef CARJNIDEMO_MP4PARSER_H
#define CARJNIDEMO_MP4PARSER_H

#endif //CARJNIDEMO_MP4PARSER_H

#include <stdio.h>



#include <stdint.h>

typedef struct {

    uint32_t Hour;
    uint32_t Minute;
    uint32_t Second;
    uint32_t Year;
    uint32_t Month;
    uint32_t Day;

    char Status;
    char NSInd;
    char EWInd;
    char reservd;
    float Latitude;
    float Longitude;
    float Speed;
    float Angle;

}RMCINFO;

typedef struct {


    int feedback;

    int timeSpan;

    RMCINFO *info;
}GPSResult;

GPSResult* parseMP4(const char *filePath);