#ifndef HTTP_HELPER_H
#define HTTP_HELPER_H

enum HttpModels_HttpMethod{
    HTTP_METHOD_UNKNOWN,
    HTTP_METHOD_GET,
    HTTP_METHOD_POST,
    HTTP_METHOD_HEAD
};

enum HttpModels_HttpVersion{
    HTTP_VERSION_UNKNOWN,
    HTTP_VERSION_0_9,
    HTTP_VERSION_1_0,
    HTTP_VERSION_1_1,
    HTTP_VERSION_2_0
};

enum HttpModels_HttpHeader{
    HTTP_HEADER___CUSTOM,

    // General Headers
    HTTP_HEADER_DATE,              // Date: Tue, 15 Nov 1994 08:12:31 GMT                  All HTTP/1.0 date/time stamps must be represented in GMT, without exception.
    HTTP_HEADER_PRAGMA,            // Pragma: no-cache

    // Entity Headers
    HTTP_HEADER_ALLOW,             // Allow: GET, POST, HEAD
    HTTP_HEADER_CONTENT_ENCODING,  // Content-Encoding: deflate, gzip                      Lists any encodings that have been applied to the payload, and in what order. This lets the recipient know how to decode the payload.
    HTTP_HEADER_CONTENT_LENGTH,    // Content-Length: 3495                                 A valid Content-Length field value is REQUIRED on all HTTP/1.0 request messages containing an entity body.
    HTTP_HEADER_CONTENT_TYPE,      // Content-Type: text/html                              Any HTTP/1.0 message containing an entity body should include a Content-Type header field defining the media type of that body.
    HTTP_HEADER_EXPIRES,           // Expires: Thu, 01 Dec 1994 16:00:00 GMT               Contains the date/time after which the response is considered expired.
    HTTP_HEADER_LAST_MODIFIED,     // Last-Modified: Tue, 15 Nov 1994 12:45:26 GMT

    // Request Headers
    HTTP_HEADER_AUTHORIZATION,     // Authorization: Basic username:password
    HTTP_HEADER_FROM,              // From: merkithuseyin@gmail.com
    HTTP_HEADER_IF_MODIFIED_SINCE, // If-Modified-Since: Sat, 29 Oct 1994 19:43:31 GMT     Used with the GET method to make it conditional: if the requested resource has not been modified since the time specified in this field, a copy of the resource will not be returned from the server; instead, a 304 (not modified) response will be returned without any Entity-Body.
    HTTP_HEADER_REFERER,           // Referer: https://somesite/some/path                  Contains the absolute or partial address from which a resource has been requested. When you click a link, the Referer contains the address of the page that includes the link.
    HTTP_HEADER_USER_AGENT,        // User-Agent: curl/7.64.1

    // Response Headers
    HTTP_HEADER_LOCATION,           // Location: https://somesite/some/path                For 3xx responses, the location MUST indicate the server's preferred URL for automatic redirection to the resource.
    HTTP_HEADER_SERVER,             // Server: Apache/2.4.1 (Unix)                         Describes the software used by the origin server that handled the request — that is, the server that generated the response.
    HTTP_HEADER_WWW_AUTHENTICATE,   // WWW-Authenticate: Basic                             MUST be included in 401 (unauthorized) response messages.
};

struct HttpModels_HttpHeaderPair{
    enum HttpModels_HttpHeader key;
    char value[200];
};

struct HttpModels_StringPair{
    char key[50];
    char value[50];
};

struct HttpModels_Resource{
    int parametersCount;
    char uri[200];
    struct HttpModels_StringPair parameters[10];
};

struct HttpModels_Request{
    /*
       ***Simple-Request (HTTP/0.9):***
       GET <SP> <URI> <CRLF>

       ***Example:***
       GET api/example/to/uri?paramKey1=paramValue1&paramKey2=paramValue2



       ***Full-Request (HTTP/1.0):***
       <Method> <SP> <URI> <SP> HTTP/1.0 <CRLF>
       <HeaderPair> <CRLF>
       ...
       <HeaderPair> <CRLF>
       <CRLF>
       <Body>

       ***Example:***
       POST api/example/to/uri?paramKey1=paramValue1&paramKey2=paramValue2 HTTP/1.0
       HeaderKey1: HeaderValue1
       HeaderKey2: HeaderValue2

       Body content here!
    */

    enum HttpModels_HttpMethod method;              // POST
    struct HttpModels_Resource resource;            // api/example/to/uri?paramKey1=paramValue1&paramKey2=paramValue2
    enum HttpModels_HttpVersion version;            // HTTP/1.0
    unsigned int headersCount;
    struct HttpModels_HttpHeaderPair headers[30];   // [0]: HeaderKey1: HeaderValue1, [1]: HeaderKey2: HeaderValue2
    unsigned int bodyLength;
    char *body;                                     // Body content here!
};

struct HttpModels_Response{
    /*
       ***Simple-Response (HTTP/0.9):***
       <Body>

       ***Example:***
       Body content here!



       ***Full-Response (HTTP/1.0):***
       HTTP/1.0 <SP> <StatusCode> [<SP> <Phrase>] <CRLF>
       <HeaderPair> <CRLF>
       ...
       <HeaderPair> <CRLF>
       <CRLF>
       <Body>

       ***Example:***
       HTTP/1.0 202 Accepted
       HeaderKey1: HeaderValue1
       HeaderKey2: HeaderValue2

       Body content here!
    */

    enum HttpModels_HttpVersion version;            // HTTP/1.0
    int statusCode;                                 // 202
    char statusPhrase[20];                          // Accepted
    unsigned int headersCount;
    struct HttpModels_HttpHeaderPair headers[30];   // [0]: HeaderKey1: HeaderValue1, [1]: HeaderKey2: HeaderValue2
    char *body;                                     // Body content here!
};

/// Parses string to HTTP request model. If it fails, then it prints error and returns 0. Otherwise returns 1 and allocates memory for httpRequest->resource and httpRequest->body (body will not be allocated in case of it doesn't exist)
int HttpHelper_string_to_httpRequest(const char *str, struct HttpModels_Request *httpRequest);

/// Parses HTTP response model to string
void HttpHelper_httpResponse_to_string(struct HttpModels_Response response, char *targetStr);

#endif //HTTP_HELPER_H