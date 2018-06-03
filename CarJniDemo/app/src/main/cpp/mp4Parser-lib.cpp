#include <jni.h>
#include <string>
#include "MP4Parser.h"

void setIntFieldValue(JNIEnv *env,jclass objclass,jobject obj,char* name ,jint value){
    jfieldID  id = env->GetFieldID(objclass,name,"I");
    env->SetIntField(obj,id,value);
}
void setFloatFieldValue(JNIEnv *env,jclass objclass,jobject obj,char* name ,jfloat value){
    jfieldID  id = env->GetFieldID(objclass,name,"F");
    env->SetFloatField(obj,id,value);
}

void setCharFieldValue(JNIEnv *env,jclass objclass,jobject obj,char* name ,jchar value){
    jfieldID  id = env->GetFieldID(objclass,name,"C");
    env->SetCharField(obj,id,value);
}
extern "C"
JNIEXPORT jobjectArray


JNICALL
Java_com_sjcam_driverecorder_gpsparser_GPSParser_parser(
        JNIEnv *env,jobject /* this */
        ,jstring path) {

    if (path == NULL)return NULL;

    const char* p = env->GetStringUTFChars(path, NULL);
    GPSResult *result = parseMP4(p);
    env->ReleaseStringUTFChars(path,p);

    if(result == NULL || result->timeSpan==0 )return NULL;

    int arraySize = result->timeSpan;
    jclass gpsModelClass = env->FindClass("com/sjcam/driverecorder/gpsparser/GPSModel");
    jobjectArray array = env->NewObjectArray(arraySize,gpsModelClass,0);

    for (int i=0;i < (result->timeSpan);i++) {

        RMCINFO info = result->info[i];
        jmethodID gpsModelId = env->GetMethodID(gpsModelClass, "<init>", "()V");
        jobject gpsModel = env->NewObjectA(gpsModelClass,gpsModelId,0);

        setIntFieldValue(env, gpsModelClass, gpsModel, "Hour",info.Hour);
        setIntFieldValue(env, gpsModelClass, gpsModel, "Minute", info.Minute);
        setIntFieldValue(env, gpsModelClass, gpsModel, "Second", info.Second);
        setIntFieldValue(env, gpsModelClass, gpsModel, "Year", info.Year);
        setIntFieldValue(env, gpsModelClass, gpsModel, "Month", info.Month);
        setIntFieldValue(env, gpsModelClass, gpsModel, "Day", info.Day);
        setCharFieldValue(env, gpsModelClass, gpsModel, "Status", info.Status);
        setCharFieldValue(env, gpsModelClass, gpsModel, "NSInd",  info.NSInd);
        setCharFieldValue(env, gpsModelClass, gpsModel, "EWInd",info.EWInd);
        setCharFieldValue(env, gpsModelClass, gpsModel, "reservd", info.reservd);
        setFloatFieldValue(env, gpsModelClass, gpsModel, "Latitude", info.Latitude);
        setFloatFieldValue(env, gpsModelClass, gpsModel, "Longitude", info.Longitude);
        setFloatFieldValue(env, gpsModelClass, gpsModel, "Speed",  info.Speed);
        setFloatFieldValue(env, gpsModelClass, gpsModel, "Angle", info.Angle);
        env->SetObjectArrayElement(array,i,gpsModel);
    }

    free(result->info);
    free(result);
    return array;
}




