// -I /u01/app/oracle/product/11.2.0.4/rdbms/public
// -L /u01/app/oracle/product/11.2.0.4/lib
// -lclntsh

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<oci.h>

void printError(OCIError* errhp){
    ub4 errcode;
    text errbuff[1024];
    OCIErrorGet((dvoid*)errhp,(ub4)1,(text*)NULL,&errcode,errbuff,(ub4)sizeof(errbuff),(ub4)OCI_HTYPE_ERROR);
    if(strlen(errbuff)!=0){
        printf("%d\n",errcode);
        printf("%s\n",errbuff);
    }
    return;
}
int main(){
    OCIEnv* envhp;
    OCIServer* servhp;
    OCIError* errhp;
    OCISession* sessionhp;
    OCISvcCtx* svcCtxhp;
    OCIStmt* stmthp;
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
    OCIHandleAlloc((dvoid*)envhp,(dvoid**)&stmthp,OCI_HTYPE_STMT,0,(dvoid**)0);
    //OraText* sql = (OraText*) "select * from oci";
    text* sql =(text*) "insert into oci values(:1,:2)";
    OCIBind* bndhp1 = NULL;
    OCIBind* bndhp2 = NULL;
    char* str = "0002";
    OCIStmtPrepare(stmthp,errhp,sql,(ub4)strlen(sql),(ub4)OCI_NTV_SYNTAX,(ub4)OCI_DEFAULT);
    OCIBindByPos(stmthp,&bndhp1,errhp,1,(void*)str,(sb4)strlen(str)+1,SQLT_STR,(void*)0,(ub2*)0,(ub2*)0,(ub4)0,(ub4*)0,OCI_DEFAULT);
    OCIBindByPos(stmthp,&bndhp2,errhp,2,(void*)str,(sb4)strlen(str)+1,SQLT_STR,(void*)0,(ub2*)0,(ub2*)0,(ub4)0,(ub4*)0,OCI_DEFAULT);
    //OCIBindByName(stmthp,&bndhp2,errhp,(text*)":userName",strlen(":userName"),(void*)str,(sb4)strlen(str)+1,SQLT_STR,(void*)0,(ub2*)0,(ub2*)0,(ub4)0,(ub4*)0,OCI_DEFAULT);
    //OCIBindByName(stmthp,&bndhp2,errhp,(text*)":tip",strlen(":tip"),(void*)str,(sb4)strlen(str)+1,SQLT_STR,(void*)0,(ub2*)0,(ub2*)0,(ub4)0,(ub4*)0,OCI_DEFAULT);
    //char param1[1024],param2[1024];
    //OCIDefine* define1 = NULL;
    //OCIDefine* define2 = NULL;
    //OCIDefineByPos(stmthp,&define1,errhp,1,(void*)param1,(sb4)1024*sizeof(char),SQLT_STR,(dvoid*)0,(ub2*)0,(ub2*)0,OCI_DEFAULT);
    //OCIDefineByPos(stmthp,&define2,errhp,2,(void*)param2,(sb4)1024*sizeof(char),SQLT_STR,(dvoid*)0,(ub2*)0,(ub2*)0,OCI_DEFAULT);
    //OCITransStart(svcCtxhp,errhp,30,OCI_TRANS_NEW);
    OCIStmtExecute(svcCtxhp,stmthp,errhp,(ub4)1,(ub4)0,(CONST OCISnapshot*)0,(OCISnapshot*)0,OCI_DEFAULT);
    sword status = OCITransCommit(svcCtxhp,errhp,OCI_DEFAULT);
    if(status == OCI_SUCCESS)
        printf("事务提交成功\n");
    //OCITransRollback(svcCtxhp,errhp,OCI_DEFAULT);
    //while(OCIStmtFetch2(stmthp,errhp,1,OCI_DEFAULT,0,OCI_DEFAULT)!=OCI_NO_DATA){
    //    printf("%s %s\n",param1,param2);
    //}
    OCISessionEnd(svcCtxhp,errhp,sessionhp,OCI_DEFAULT); //结束会话
    OCIServerDetach(servhp,errhp,OCI_DEFAULT);  //断开连接
    OCIHandleFree((dvoid*)envhp,OCI_HTYPE_ENV); //释放句柄
    return 0;
}

