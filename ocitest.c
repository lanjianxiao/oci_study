// -I /u01/app/oracle/product/11.2.0.4/rdbms/public
// -L /u01/app/oracle/product/11.2.0.4/lib
// -lclntsh

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<oci.h>

int main(){
    OCIEnv* envhp;
    OCIServer* servhp;
    OCIError* errhp;
    OCISession* sessionhp;
    OCISvcCtx* svcCtxhp;
    OCIEnvCreate(&envhp,OCI_THREADED|OCI_OBJECT,(dvoid*)0,0,0,0,(size_t)0,(dvoid**)0);
    OCIHandleAlloc((dvoid*)envhp,(dvoid**)&servhp,OCI_HTYPE_SERVER,0,(dvoid**)0);
    OCIAttrSet((dvoid*)servhp,OCI_HTYPE_SERVER,(dvoid*)"FALSE",(ub4)0,OCI_ATTR_NONBLOCKING_MODE,errhp);
    OCIHandleAlloc((dvoid*)envhp,(dvoid**)&errhp,OCI_HTYPE_ERROR,0,(dvoid**)0);
    if(OCIServerAttach(servhp,errhp,"192.168.100.40:1521/dbutf",strlen("192.168.100.40:1521/dbutf"),OCI_DEFAULT)==OCI_SUCCESS){
        printf("connect oracle server success\n");
    }
    OCIHandleAlloc((dvoid*)envhp,(dvoid**)&svcCtxhp,OCI_HTYPE_SVCCTX,0,(dvoid**)0);
    OCIAttrSet((dvoid*)svcCtxhp,OCI_HTYPE_SVCCTX,(dvoid*)servhp,(ub4)0,OCI_ATTR_SERVER,errhp);
    OCIHandleAlloc((dvoid*)envhp,(dvoid**)&sessionhp,OCI_HTYPE_SESSION,0,(dvoid**)0);
    OCIAttrSet((dvoid*)sessionhp,OCI_HTYPE_SESSION,(dvoid*)"pms",(ub4)strlen("pms"),OCI_ATTR_USERNAME,errhp);
    OCIAttrSet((dvoid*)sessionhp,OCI_HTYPE_SESSION,(dvoid*)"pms",(ub4)strlen("pms"),OCI_ATTR_PASSWORD,errhp);
    if(OCISessionBegin((dvoid*)svcCtxhp,errhp,sessionhp,OCI_CRED_RDBMS,OCI_DEFAULT)==OCI_SUCCESS){
        printf("connect oracle db succcess\n");
    }else{
        printf("connect oracle db failed\n");
        ub4 errcode;
        text errbuff[1024];
        OCIErrorGet((dvoid*)errhp,(ub4)1,(text*)NULL,&errcode,errbuff,(ub4)sizeof(errbuff),(ub4)OCI_HTYPE_ERROR);
        printf("%s\n",errbuff);
        return -1;
    }
    OCIAttrSet((dvoid*)svcCtxhp,OCI_HTYPE_SVCCTX,(dvoid*)sessionhp,(ub4)0,OCI_ATTR_SESSION,errhp);
    OCISessionEnd(svcCtxhp,errhp,sessionhp,OCI_DEFAULT);
    OCIServerDetach(servhp,errhp,OCI_DEFAULT);
    OCIHandleFree((dvoid*)envhp,OCI_HTYPE_ENV);
    return 0;
}
