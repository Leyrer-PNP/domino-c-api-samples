PGM
     MONMSG MSGID(CPF2103)

     CRTCMOD MODULE(QNTCSDK/USERREG) +
          SRCSTMF('/QNTCSDK/SAMPLES/ADMIN/USERREG/USERREG.C') +
          SYSIFCOPT(*IFSIO) +
          DBGVIEW(*ALL) +
          DEFINE(OS400 UNIX) +
          TGTCCSID(37) +
          TERASPACE(*YES) +
          STGMDL(*INHERIT) +
          INCDIR('/QIBM/PRODDATA/QADRT/INCLUDE' '/QIBM/PRODDATA/LOTUS/NOTESAPI/INCLUDE')

     CRTPGM PGM(QNTCSDK/USERREG) +
          MODULE(*PGM) +
          ENTMOD(QADRT/QADRTMAIN2) +
          BNDSRVPGM(QNOTES/LIBNOTES *LIBL/QADRTTS) +
          OPTION(*DUPVAR *DUPPROC) +
          DETAIL(*BASIC)

ENDPGM
