#include <ctype.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "helper.h"

/// Moves pointer to next character until a non-space character or 'exclusion' encountered. Returns 1 if exclusion encountered, otherwise 0. Don't forget to save initial pointer if given string was allocated dynamically
static int trim_string(char **str, char exclusion){
    for (int i = 0; **str; ++i) {
        if (**str == exclusion) return 1;
        if (isspace(**str)){
            (*str)++;
            i--;
        }
        else return 0;
    }
    return 0;
}

static enum HttpModels_HttpMethod string_to_httpMethod(const char *str) {
    size_t strLen = strlen(str);

    for (int i = 0; str[i]; ++i) {
        if (isspace(str[i])) continue;

        // GET
        if (i + 2 < strLen && str[i] == 'G' && str[i + 1] == 'E' && str[i + 2] == 'T') {
            return HTTP_METHOD_GET;
        }

        // POST
        if (i + 3 < strLen && str[i] == 'P' && str[i + 1] == 'O' && str[i + 2] == 'S' && str[i + 3] == 'T') {
            return HTTP_METHOD_POST;
        }

        // HEAD
        if (i + 3 < strLen && str[i] == 'H' && str[i + 1] == 'E' && str[i + 2] == 'A' && str[i + 3] == 'D') {
            return HTTP_METHOD_HEAD;
        }
    }

    return HTTP_METHOD_UNKNOWN;
}

static int string_to_httpResource(const char *str, struct HttpModels_Resource *httpResource) {
    char localStr[strlen(str)];
    strcpy(localStr, str);
    size_t strLen = strlen(localStr);

    httpResource->parametersCount = 0;
    strcpy(httpResource->uri, "/");

    if (strLen == 0 || localStr[0] != '/') {
        fprintf(stderr, "Resource path must start with '/' character \r\n"); //
        return 0;
    }

    if (strLen == 1){
        return 1;   // "GET / HTTP/1.0" is valid request with valid resource uri
    }

    int questionMarkIndex = -1;
    int ampersandCharCount = 0;
    int isNumberSignEncountered = 0;
    for (int i = 0; localStr[i]; ++i) {
        if (isNumberSignEncountered){
            localStr[i] = '\0';
            continue;
        }

        if (localStr[i] == '#'){
            isNumberSignEncountered = 1;
            localStr[i] = '\0';
            continue;
        }

        if (questionMarkIndex == -1 && localStr[i] == '?') {
            questionMarkIndex = i;
        }

        if (localStr[i] == '&') ampersandCharCount++;
    }

    strLen = strlen(localStr);

    int maxParameters = sizeof httpResource->parameters / sizeof httpResource->parameters[0];
    if (ampersandCharCount + 1 > maxParameters){
        fprintf(stderr, "Too many parameters\r\n");
        return 0;
    }

    int maxUriLength = sizeof(httpResource->uri) / sizeof(httpResource->uri[0]);
    if (questionMarkIndex < 0) {
        if (strLen + 1 > maxUriLength){
            fprintf(stderr, "Uri too long\r\n");
            return 0;
        }

        strcpy(httpResource->uri, localStr);
        return 1;
    }

    char *localStrPointer = malloc(strLen + 1);
    char *localStrPointerInitialAddress = localStrPointer;
    strcpy(localStrPointer, localStr);
    char *uri = strsep(&localStrPointer, "?");
    if (strlen(uri) + 1 > maxUriLength){
        fprintf(stderr, "Uri too long\r\n");
        free(localStrPointerInitialAddress);
        return 0;
    }
    strcpy(httpResource->uri , uri);

    int maxParameterKeyLength = sizeof(httpResource->parameters->key) / sizeof(httpResource->parameters->key[0]);
    int maxParameterValueLength = sizeof(httpResource->parameters->value) / sizeof(httpResource->parameters->value[0]);

    for (int i = 0; i < ampersandCharCount + 1; ++i) {
        char *parameter = strsep(&localStrPointer, "&");
        char *key = strsep(&parameter, "=");

        if (parameter == NULL || strlen(parameter) == 0){
            fprintf(stderr, "Invalid http parameter\r\n");
            free(localStrPointerInitialAddress);
            return 0;
        }

        if (strlen(key) + 1 > maxParameterKeyLength ){
            fprintf(stderr, "Parameter key too long\r\n");
            free(localStrPointerInitialAddress);
            return 0;
        }
        strcpy(httpResource->parameters[i].key, key);

        if (strlen(parameter) + 1 > maxParameterValueLength ){
            fprintf(stderr, "Parameter value too long\r\n");
            free(localStrPointerInitialAddress);
            return 0;
        }

        strcpy(httpResource->parameters[i].value, parameter);
        httpResource->parametersCount++;
    }
    free(localStrPointerInitialAddress);
    return 1;
}

static enum HttpModels_HttpVersion string_to_httpVersion(const char *str) {
    if(strlen(str) < 8) return HTTP_VERSION_UNKNOWN;

    char *localStrPointer = malloc(strlen(str) + 1);
    char *localStrPointerInitialAddress = localStrPointer;
    strcpy(localStrPointer, str);

    trim_string(&localStrPointer, '\0');

    enum HttpModels_HttpVersion result = HTTP_VERSION_UNKNOWN;
    if (strncmp(localStrPointer, "HTTP/0.9", 8) == 0) result = HTTP_VERSION_0_9;
    else if (strncmp(localStrPointer, "HTTP/1.0", 8) == 0) result = HTTP_VERSION_1_0;
    else if (strncmp(localStrPointer, "HTTP/1.1", 8) == 0) result = HTTP_VERSION_1_1;
    else if (strncmp(localStrPointer, "HTTP/2.0", 8) == 0) result = HTTP_VERSION_2_0;

    free(localStrPointerInitialAddress);
    return result;
}

static int string_to_httpHeaderPair(const char *str, struct HttpModels_HttpHeaderPair *headerPair){
    if (strlen(str) == 0){
        fprintf(stderr, "Invalid http header pair\r\n");
        return 0;
    }

    char *localStrPointer = malloc(strlen(str) + 1);
    char *localStrPointerInitialAddress = localStrPointer;
    strcpy(localStrPointer, str);

    trim_string(&localStrPointer, '\0');

    char *headerKey = strsep(&localStrPointer, ":");
    if (headerKey == NULL || strlen(headerKey) == 0){
        fprintf(stderr, "Invalid http header pair: %s\r\n", str);
        free(localStrPointerInitialAddress);
        return 0;
    }

    for (int i = 0; headerKey[i] ; ++i) {
        if (isspace(headerKey[i])){
            headerKey[i] = '\0';
            break;
        }
        headerKey[i] = (char) tolower(headerKey[i]);                 // header field is case-insensitive
    }

    if (strlen(headerKey) == 5 && strcmp(headerKey, "allow") == 0) headerPair->key = HTTP_HEADER_ALLOW;
    else if (strlen(headerKey) == 13 && strcmp(headerKey, "authorization") == 0) headerPair->key = HTTP_HEADER_AUTHORIZATION;
    else if (strlen(headerKey) == 16 && strcmp(headerKey, "content-encoding") == 0) headerPair->key = HTTP_HEADER_CONTENT_ENCODING;
    else if (strlen(headerKey) == 14 && strcmp(headerKey, "content-length") == 0) headerPair->key = HTTP_HEADER_CONTENT_LENGTH;
    else if (strlen(headerKey) == 12 && strcmp(headerKey, "content-type") == 0) headerPair->key = HTTP_HEADER_CONTENT_TYPE;
    else if (strlen(headerKey) == 4 && strcmp(headerKey, "date") == 0) headerPair->key = HTTP_HEADER_DATE;
    else if (strlen(headerKey) == 7 && strcmp(headerKey, "expires") == 0) headerPair->key = HTTP_HEADER_EXPIRES;
    else if (strlen(headerKey) == 4 && strcmp(headerKey, "from") == 0) headerPair->key = HTTP_HEADER_FROM;
    else if (strlen(headerKey) == 17 && strcmp(headerKey, "if-modified-since") == 0) headerPair->key = HTTP_HEADER_IF_MODIFIED_SINCE;
    else if (strlen(headerKey) == 13 && strcmp(headerKey, "last-modified") == 0) headerPair->key = HTTP_HEADER_LAST_MODIFIED;
    else if (strlen(headerKey) == 8 && strcmp(headerKey, "location") == 0) headerPair->key = HTTP_HEADER_LOCATION;
    else if (strlen(headerKey) == 6 && strcmp(headerKey, "pragma") == 0) headerPair->key = HTTP_HEADER_PRAGMA;
    else if (strlen(headerKey) == 7 && strcmp(headerKey, "referer") == 0) headerPair->key = HTTP_HEADER_REFERER;
    else if (strlen(headerKey) == 6 && strcmp(headerKey, "server") == 0) headerPair->key = HTTP_HEADER_SERVER;
    else if (strlen(headerKey) == 10 && strcmp(headerKey, "user-agent") == 0) headerPair->key = HTTP_HEADER_USER_AGENT;
    else if (strlen(headerKey) == 16 && strcmp(headerKey, "www-authenticate") == 0) headerPair->key = HTTP_HEADER_WWW_AUTHENTICATE;
    else headerPair->key = HTTP_HEADER___CUSTOM;

    trim_string(&localStrPointer, '\0');

    if (headerPair->key == HTTP_HEADER___CUSTOM) {
        // In case of custom header detected, store both header key and value pairs on "pair.value", for example: headerPair->key = HTTP_HEADER___CUSTOM, headerPair->value = "customHeaderKey:customHeaderValue"

        char headerKeyValueStr[strlen(headerKey) + strlen(localStrPointer) + 1];
        strcpy(headerKeyValueStr, headerKey);
        strlcat(headerKeyValueStr, ":", strlen(headerKeyValueStr) + 2);

        size_t headerValueSize = (sizeof headerPair->value / sizeof headerPair->value[0]) ;
        size_t newExpectedSize = strlen(headerKeyValueStr) + strlen(localStrPointer) + 1;
        strlcat(headerKeyValueStr, localStrPointer, headerValueSize < newExpectedSize ? headerValueSize : newExpectedSize);
        strcpy(headerPair->value, headerKeyValueStr);
    }
    else strcpy(headerPair->value, localStrPointer);

    free(localStrPointerInitialAddress);
    return 1;
}

int HttpHelper_string_to_httpRequest(const char *str, struct HttpModels_Request *httpRequest){
    char *localStrPointer = malloc(strlen(str) + 1);
    char *localStrPointerInitialAddress = localStrPointer;
    strcpy(localStrPointer, str);

    // Method
    trim_string(&localStrPointer, '\0');
    char *httpMethodString = strsep(&localStrPointer, " ");
    enum HttpModels_HttpMethod httpMethod = string_to_httpMethod(httpMethodString);
    if (httpMethod == HTTP_METHOD_UNKNOWN){
        fprintf(stderr, "Unknown http method: %s\r\n", httpMethodString);
        free(localStrPointerInitialAddress);
        return 0;
    }
    httpRequest->method = httpMethod;

    // Resource
    trim_string(&localStrPointer, '\0');
    char *httpResourceString = strsep(&localStrPointer, " \n");
    struct HttpModels_Resource *httpResource = malloc(sizeof (struct HttpModels_Resource));
    int isSuccess = string_to_httpResource(httpResourceString, httpResource);
    if (!isSuccess){
        fprintf(stderr, "Couldn't cast resource: %s\r\n", httpResourceString);
        free(localStrPointerInitialAddress);
        free(httpResource);
        return 0;
    }
    httpRequest->resource=*httpResource;

    httpRequest->bodyLength = 0;
    httpRequest->headersCount = 0;

    // Version
    char *httpVersionString = strsep(&localStrPointer, "\n");
    if (httpVersionString != NULL)
        trim_string(&httpVersionString, '\0');

    if (httpVersionString == NULL || strcmp(httpVersionString, "") == 0){
        // If the protocol version is not specified, the recipient must assume that the message is in the simple HTTP/0.9 format.
        httpRequest->version = HTTP_VERSION_0_9;
        free(localStrPointerInitialAddress);
        return 1;
    }

    httpRequest->version = string_to_httpVersion(httpVersionString);
    if (httpRequest->version == HTTP_VERSION_UNKNOWN){
        fprintf(stderr, "Unknown http version: %s\r\n", httpVersionString);
        free(localStrPointerInitialAddress);
        free(httpResource);
        return 0;
    }

    // Headers
    int maxHeaders = sizeof httpRequest->headers / sizeof httpRequest->headers[0];
    for (int i = 0; i < maxHeaders; ++i) {
        int isLineFeedEncountered = trim_string(&localStrPointer, '\n');
        if (isLineFeedEncountered) break;

        char *headerString = strsep(&localStrPointer, "\n");
        isSuccess = string_to_httpHeaderPair(headerString, &httpRequest->headers[i]);
        if (!isSuccess){
            fprintf(stderr, "Couldn't cast header: %s\r\n", headerString);
            free(localStrPointerInitialAddress);
            free(httpResource);
            return 0;
        }
        httpRequest->headersCount++;

        if (httpRequest->headers[i].key == HTTP_HEADER_CONTENT_LENGTH){

            // get rid of whitespaces
            for (int j = 0; httpRequest->headers[i].value[j] ; ++j)
                if (isspace(httpRequest->headers[i].value[j])) httpRequest->headers[i].value[j] = '\0';

            httpRequest->bodyLength = (int) strtonum(httpRequest->headers[i].value, 0, 10000, NULL);
        }
    }

    /*
     Requests must have CRLF after request-line. If headers exit, then it must be after headers pairs. And then body comes after this CRLF:
     Full-Request = Request-Line
                    *( General-Header | Request-Header | Entity-Header )
                    CRLF
                    [ Entity-Body ]


     — RFC 1945: 4.HTTP Message: 4.1 Message Types
     */

    // Check LF instead of CRLF to tolerate requests which uses LF as 'new line'. Note that according to RFC 1945, CRLF must be included but checking only LF includes this case as well.
    if (*localStrPointer != '\n'){
        fprintf(stderr, "Invalid http request, couldn't detect Line-Feed after request-line/headers\r\n");
        free(localStrPointerInitialAddress);
        free(httpResource);
        return 0;
    }

    // pass LF
    localStrPointer++;

    // Body
    if (httpRequest->bodyLength <= 0){
        free(localStrPointerInitialAddress);
        return 1;
    }

    httpRequest->body = malloc(httpRequest->bodyLength + 1);
    httpRequest->body[httpRequest->bodyLength] = '\0';

    for (int i = 0; i < httpRequest->bodyLength; ++i){
        httpRequest->body[i] = *localStrPointer;
        if (*localStrPointer++ == '\0') break;
    }

    free(localStrPointerInitialAddress);
    return 1;
}

void HttpHelper_httpResponse_to_string(struct HttpModels_Response response, char *targetStr){
    /*
        HTTP/1.0 202 Accepted
        HeaderKey1: HeaderValue1
        HeaderKey2: HeaderValue2

        Body content here!
    */

    switch (response.version) {
        case HTTP_VERSION_UNKNOWN:
        case HTTP_VERSION_0_9:
            strcpy(targetStr, response.body);
            return;
        case HTTP_VERSION_1_0:
        case HTTP_VERSION_1_1:
        case HTTP_VERSION_2_0:
            strcpy(targetStr, "HTTP/1.0 ");
            break;
    }
    // HTTP/1.0

    char statusCodeString[5];
    sprintf(statusCodeString, "%d ", response.statusCode);
    strlcat(targetStr, statusCodeString, strlen(targetStr) + strlen(statusCodeString) + 1);
    // HTTP/1.0 202

    if (*response.statusPhrase && strlen(response.statusPhrase) > 0)
        strlcat(targetStr, response.statusPhrase, strlen(targetStr) + strlen(response.statusPhrase) + 1);
    // HTTP/1.0 202 Accepted

    strlcat(targetStr, " \r\n", strlen(targetStr) + 4);
    // HTTP/1.0 202 Accepted <CRLF>

    for (int i = 0; response.headersCount; ++i) {
        char *headerPair;
        switch (response.headers[i].key) {
            case HTTP_HEADER_DATE:
                headerPair = "Date: ";
                break;
            case HTTP_HEADER_PRAGMA:
                headerPair = "Pragma: ";
                break;
            case HTTP_HEADER_ALLOW:
                headerPair = "Allow: ";
                break;
            case HTTP_HEADER_CONTENT_ENCODING:
                headerPair = "Content-Encoding: ";
                break;
            case HTTP_HEADER_CONTENT_LENGTH:
                headerPair = "Content-Length: ";
                break;
            case HTTP_HEADER_CONTENT_TYPE:
                headerPair = "Content-Type: ";
                break;
            case HTTP_HEADER_EXPIRES:
                headerPair = "Expires: ";
                break;
            case HTTP_HEADER_LAST_MODIFIED:
                headerPair = "Last-Modified: ";
                break;
            case HTTP_HEADER_AUTHORIZATION:
                headerPair = "Authorization: ";
                break;
            case HTTP_HEADER_FROM:
                headerPair = "From: ";
                break;
            case HTTP_HEADER_IF_MODIFIED_SINCE:
                headerPair = "If-Modified-Since: ";
                break;
            case HTTP_HEADER_REFERER:
                headerPair = "Referer: ";
                break;
            case HTTP_HEADER_USER_AGENT:
                headerPair = "User-Agent: ";
                break;
            case HTTP_HEADER_LOCATION:
                headerPair = "Location: ";
                break;
            case HTTP_HEADER_SERVER:
                headerPair = "Server: ";
                break;
            case HTTP_HEADER_WWW_AUTHENTICATE:
                headerPair = "WWW-Authenticate: ";
                break;
            case HTTP_HEADER___CUSTOM:
                headerPair = "";
                break;
        }

        strlcat(headerPair, response.headers[i].value, strlen(headerPair) + strlen(response.headers[i].value) + 1);
        strlcat(targetStr, " \r\n", strlen(targetStr) + 4);
    }

    // Blank line
    strlcat(targetStr, "\r\n", strlen(targetStr) + 3);

    if (*response.body && strlen(response.body) > 0){
        strlcat(targetStr, response.body, strlen(targetStr) + strlen(response.body) + 1);
    }
}