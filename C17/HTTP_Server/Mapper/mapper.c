#include <string.h>
#include <stdlib.h>
#include "mapper.h"
#include "../Helper/helper.h"

/*
   Map your resources in this file:

   1. Write your handler function.
   2. Add your handler function into handler manager.
*/


// Handler functions
static void handler_status_http09(struct HttpModels_Response *response){
    response->version = HTTP_VERSION_0_9;
    response->statusCode = 200;
    char responseBodyString[] = "Server running...";
    response->body = malloc(strlen(responseBodyString) + 1);
    strcpy(response->body, responseBodyString);
}

static void handler_status_http10(struct HttpModels_Response *response){
    response->statusCode = 200;
    strcpy(response->statusPhrase, "OK");
    char responseBodyString[] = "Server running...";
    response->body = malloc(strlen(responseBodyString) + 1);
    strcpy(response->body, responseBodyString);
}

// Handler manager
static struct HttpModels_Response handle_request(const char *requestString){
    struct HttpModels_Request request;
    struct HttpModels_Response response;
    response.version = HTTP_VERSION_1_0;
    response.headersCount = 0;
    response.body = NULL;

    int isSuccess = HttpHelper_string_to_httpRequest(requestString, &request);
    if(!isSuccess){
        response.statusCode = 400;
        return response;
    }

    // Add your endpoints here
    if(strcmp(request.resource.uri, "/api/status") == 0 && request.method == HTTP_METHOD_GET){
        if(request.version == HTTP_VERSION_UNKNOWN || request.version == HTTP_VERSION_0_9){
            handler_status_http09(&response);
            return response;
        }
        else{
            handler_status_http10(&response);
            return response;
        }
    }
    else{
        response.statusCode = 404;
        return response;
    }
}

void HttpMapper_handle_request(const char *requestString, char **responseString){
    struct HttpModels_Response response = handle_request(requestString);
    *responseString = malloc( sizeof (response) + 1 );

    if (response.statusCode == 400){
        strcpy(*responseString, "HTTP/1.0 400 Bad Request\r\n\r\n");
    }
    else if (response.statusCode == 404){
        strcpy(*responseString, "HTTP/1.0 404 Not Found\r\n\r\nUri resource not found");
    }
    else{
        HttpHelper_httpResponse_to_string(response, *responseString);
    }

    free(response.body);
}