#include <iostream>
#include <thread>
#include <atomic>


#define _CUPS_IPP_PRIVATE_H_
#include <cups/cups.h>
#include <cups/ipp.h>


#define CUPS_PRINTER "PDF-PRINTER"

namespace global {

    std::thread *other_printer_ctrl_thread = nullptr;
    std::atomic<bool> other_printer_ctrl_running;

}  // global

/*
 * 'get_printer_type()' - Determine the printer type and URI.
 */

static cups_ptype_t			/* O - printer-type value */
get_printer_type(http_t *http,		/* I - Server connection */
                 const char   *printer,	/* I - Printer name */
                 char   *uri,		/* I - URI buffer */
                 size_t urisize)	/* I - Size of URI buffer */
{
    ipp_t			*request,	/* IPP request */
        *response;	/* IPP response */
    ipp_attribute_t	*attr;		/* printer-type attribute */
    cups_ptype_t		type;		/* printer-type value */


    /*
     * Build a GET_PRINTER_ATTRIBUTES request, which requires the following
     * attributes:
     *
     *    attributes-charset
     *    attributes-natural-language
     *    printer-uri
     *    requested-attributes
     *    requesting-user-name
     */

    httpAssembleURIf(HTTP_URI_CODING_ALL, uri, (int)urisize, "ipp", NULL, "localhost", ippPort(), "/printers/%s", printer);
    request = ippNewRequest(IPP_OP_GET_PRINTER_ATTRIBUTES);
    ippAddString(request, IPP_TAG_OPERATION, IPP_TAG_URI,
                 "printer-uri", NULL, uri);
    ippAddString(request, IPP_TAG_OPERATION, IPP_TAG_KEYWORD,
                 "requested-attributes", NULL, "printer-type");
    ippAddString(request, IPP_TAG_OPERATION, IPP_TAG_NAME,
                 "requesting-user-name", NULL, cupsUser());
    /*
     * Do the request...
     */

    response = cupsDoRequest(http, request, "/");
    if ((attr = ippFindAttribute(response, "printer-type",
                                 IPP_TAG_ENUM)) != NULL)
    {
        type = (cups_ptype_t)attr->values[0].integer;

        if (type & CUPS_PRINTER_CLASS)
            httpAssembleURIf(HTTP_URI_CODING_ALL, uri, (int)urisize, "ipp", NULL, "localhost", ippPort(), "/classes/%s", printer);
    }
    else
        type = CUPS_PRINTER_LOCAL;

    ippDelete(response);

    return (type);
}

static void set_printer_white_list(const char *printer, const char *names)
{
    return;
    ipp_t	*request;                  /* IPP Request */
    char uri[HTTP_MAX_URI] = {0}; /* URI for printer/class */
    http_t *http = NULL;

    if (get_printer_type(http, printer, uri, sizeof(uri)) & CUPS_PRINTER_CLASS)
        request = ippNewRequest(IPP_OP_CUPS_ADD_MODIFY_CLASS);
    else
        request = ippNewRequest(IPP_OP_CUPS_ADD_MODIFY_PRINTER);

    std::cout << uri << "\n";

    request = ippNewRequest(IPP_OP_CUPS_ADD_MODIFY_PRINTER);

    ippAddString(request, IPP_TAG_OPERATION, IPP_TAG_URI, "printer-uri", NULL, uri);
    ippAddString(request, IPP_TAG_OPERATION, IPP_TAG_NAME, "requesting-user-name", NULL, cupsUser());

    ippAddString(request, IPP_TAG_PRINTER, IPP_TAG_NAME, "requesting-user-name-allowed", NULL, names);

    ippDelete(cupsDoRequest(http, request, "/admin/"));

}

static void other_printer_disable()
{
    int i;
    cups_dest_t *dests, *dest;
    int num_dests = cupsGetDests(&dests);

    for (i = num_dests, dest = dests; i > 0; i --, dest ++) {
        if (strcmp(dest->name, CUPS_PRINTER) != 0) {
            set_printer_white_list(dest->name, "lp");
        }
    }
    cupsFreeDests(num_dests, dests);
}
static void other_printer_enable()
{
    int i;
    cups_dest_t *dests, *dest;
    int num_dests = cupsGetDests(&dests);

    for (i = num_dests, dest = dests; i > 0; i --, dest ++) {
        if (strcmp(dest->name, CUPS_PRINTER) != 0) {
            set_printer_white_list(dest->name, "all");
        }
    }
    cupsFreeDests(num_dests, dests);
}


static int other_printer_ctrl_worker()	/* I - Destinations */
{
    int		i, j;			/* Looping vars */
    ipp_t	*request;		/* IPP Request */
    ipp_t *response;		/* IPP Response */
    ipp_attribute_t *attr;		/* Current attribute */
    const char	*printer;		/* Printer name */
    ipp_attribute_t *allowed;		/* requesting-user-name-allowed */
    ipp_attribute_t *denied;		/* requestint-user-name-denied */

    static const char *pattrs[] =		/* Attributes we need for printers... */
        {
            "printer-name",
            "requesting-user-name-allowed",
            "requesting-user-name-denied"
        };
    request = ippNewRequest(CUPS_GET_PRINTERS);

    ippAddStrings(request, IPP_TAG_OPERATION, IPP_TAG_KEYWORD,
                  "requested-attributes", sizeof(pattrs) / sizeof(pattrs[0]),
                  NULL, pattrs);

    ippAddString(request, IPP_TAG_OPERATION, IPP_TAG_NAME, "requesting-user-name",
                 NULL, "root");

    /*
     * Do the request and get back a response...
     */

    response = cupsDoRequest(CUPS_HTTP_DEFAULT, request, "/");

    if (cupsLastError() == IPP_STATUS_ERROR_BAD_REQUEST ||
        cupsLastError() == IPP_STATUS_ERROR_VERSION_NOT_SUPPORTED)
    {
        printf("%s: Error - add '/version=1.1' to server name.", "lpstat");
        ippDelete(response);
        return (1);
    }
    else if (cupsLastError() > IPP_STATUS_OK_CONFLICTING)
    {
        printf("lpstat: %s", cupsLastErrorString());
        ippDelete(response);
        return (1);
    }

    if (response)
    {
        /*
         * Loop through the printers returned in the list and display
         * their status...
         */

        for (attr = response->attrs; attr != NULL; attr = attr->next)
        {
            /*
             * Skip leading attributes until we hit a job...
             */

            while (attr != NULL && attr->group_tag != IPP_TAG_PRINTER)
                attr = attr->next;

            if (attr == NULL)
                break;

            /*
             * Pull the needed attributes from this job...
             */

            printer     = NULL;
            allowed     = NULL;
            denied      = NULL;

            while (attr != NULL && attr->group_tag == IPP_TAG_PRINTER)
            {
                if (!strcmp(attr->name, "printer-name") &&
                    attr->value_tag == IPP_TAG_NAME)
                    printer = attr->values[0].string.text;
                else if (!strcmp(attr->name, "requesting-user-name-allowed") &&
                         attr->value_tag == IPP_TAG_NAME)
                    allowed = attr;
                else if (!strcmp(attr->name, "requesting-user-name-denied") &&
                         attr->value_tag == IPP_TAG_NAME)
                    denied = attr;
                attr = attr->next;
            }

            /*
             * See if we have everything needed...
             */

            if (printer == NULL) {
                if (attr == NULL)
                    break;
                else
                    continue;
            }
            if (allowed) {
                if (strcmp(printer, CUPS_PRINTER) == 0) {
                    set_printer_white_list(printer, "all");
                }
                else {
                    if (allowed->num_values != 1) {
                        set_printer_white_list(printer, "lp");
                    } else if (strcmp(allowed->values[0].string.text, "lp") != 0) {
                        set_printer_white_list(printer, "lp");
                    }
                }
            }
            else if (denied) {
                if (strcmp(printer, CUPS_PRINTER) == 0) {
                    set_printer_white_list(printer, "all");
                }
                else {
                    set_printer_white_list(printer, "lp");
                }
            }
            else {
                if (strcmp(printer, CUPS_PRINTER) == 0) {
                    ;
                } else {
                    set_printer_white_list(printer, "lp");
                }
            }

            if (attr == NULL)
                break;
        }

        ippDelete(response);
    }

    return (0);
}
void other_printer_ctrl_start()
{
    global::other_printer_ctrl_running = true;
    global::other_printer_ctrl_thread = new std::thread([] {
                                                            cupsSetUser("root");
                                                            other_printer_disable();
                                                            while(global::other_printer_ctrl_running) {
                                                                other_printer_ctrl_worker();
                                                                sleep(5);
                                                            }
                                                            cupsSetUser("lp");
                                                            other_printer_enable();
                                                        });

}

void other_printer_ctrl_stop()
{
    global::other_printer_ctrl_running = false;
    if (global::other_printer_ctrl_thread->joinable())
        global::other_printer_ctrl_thread->join();
}
/* 返回值
 * -1 : 未找到该设备
 * -2 : 获取打印机状态出错
 * 3 : enable  启用
 * 4 : processing 运行中
 * 5 : disable 禁用
 */
int get_printer_status(const char *printer)
{
    int i;
    cups_dest_t *dests, *dest;
    cupsSetUser("root");
    int num_dests = cupsGetDests(&dests);
    for (i = num_dests, dest = dests; i > 0; i --, dest ++) {
        if (strcmp(dest->name, printer) == 0) {
            cups_option_t *option = dest->options;
            for (int j = 0; j < dest->num_options; j++, option++) {
                if (strcmp(option->name, "printer-state") == 0)
                    return atoi(option->value);
            }
            return -2;
        }
    }
    cupsFreeDests(num_dests, dests);
    return -1;
}

int main(int argc, char *argv[])
{
    other_printer_ctrl_worker();
    // set_printer_white_list(argv[1], argv[2]);
}
