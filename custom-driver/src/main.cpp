#include "libidl.h"
#include "example.h"
#include <pthread.h>

#ifndef CDNAME
#error Must define CDNAME when compiling this main.cpp file
#endif

int main(void)
{
    char conf_path[256] ="/var/apollo/data/" CDNAME "/" CDNAME "-idl.conf";
    pthread_t IdiRxThread;

    Idl *idl = IdlNew();		// Create a new driver instance

    /* Register the CDNAME Driver Callback Routines with the IAP Driver Library (IDL) */
	
    IdlDevCreateCallbackSet(idl, dev_create_cb);
    IdlDevProvisionCallbackSet(idl, dev_provision_cb);
    IdlDevDeprovisionCallbackSet(idl, dev_deprovision_cb);
    IdlDevReplaceCallbackSet(idl, dev_replace_cb);
    IdlDevDeleteCallbackSet(idl, dev_delete_cb);
    IdlDpReadCallbackSet(idl, dp_read_cb);
    IdlDpWriteCallbackSet(idl, dp_write_cb);

    if (IdiStart() == 0) {
        printf("The " CDNAME " IDL Driver started up...\r\n");
		
		/* Create any POSIX threads your driver might need before calling  */
		/* IdlInit() as IdlInit() never returns.  For example, this driver */
		/* creates a thread that could be used to process asynchronous     */
		/* communications packets the driver needs to capture and process. */

        pthread_create( &IdiRxThread, NULL, IdiRxThreadFunction, NULL);

		/* Initialize the IDL CDNAME Driver using the parameters defined */
		/* in CDNAME-idl.conf configuration file. IdlInit() never exits. */

        IdlInit(conf_path, idl);
    }
    return 0;
}

