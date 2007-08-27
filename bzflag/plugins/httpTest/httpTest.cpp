// httpTest.cpp : Defines the entry point for the DLL application.
//

#include <map>
#include "bzfsAPI.h"
#include "plugin_utils.h"
#include "plugin_HTTP.h"

BZ_GET_PLUGIN_VERSION

class HTTPServer : public BZFSHTTPServer
{
public:
  HTTPServer( const char * plugInName ): BZFSHTTPServer(plugInName){};

  virtual bool acceptURL ( const char *url );
  virtual void getURLData ( const char* url, int requestID );

  std::string dir;
};

HTTPServer httpServer("httpTest");

BZF_PLUGIN_CALL int bz_Load ( const char* /*commandLine*/ )
{
  bz_debugMessage(4,"httpTest plugin loaded");
  httpServer.startupHTTP();
  return 0;
}

BZF_PLUGIN_CALL int bz_Unload ( void )
{
  httpServer.shutdownHTTP();
  bz_debugMessage(4,"httpTest plugin unloaded");
 return 0;
}

bool HTTPServer::acceptURL ( const char *url )
{
  if (bz_BZDBItemExists("HTTPTestDir"))
    dir = bz_getBZDBString("HTTPTestDir").c_str();
  else
    dir = "./";
  return true;
}

void HTTPServer::getURLData ( const char* url, int requestID )
{
  std::string URL = url;
  if ( URL.size() < 2 )
  {
    std::string crapPage = "This Is data from HTTP via BZFS\r\n\r\n";
    setURLDataSize ( (unsigned int)crapPage.size(), requestID );
    setURLData ( crapPage.c_str(), requestID );
  }
  else
  {
    if (*(URL.end()-1) == '/')
      URL += "index.html";

    std::string path = dir + URL;
    FILE *fp = fopen(path.c_str(),"rb");
    if (!fp)
    {
      std::string crapPage = "404: Page Not found (from BZFS)\r\n\r\n";
      setURLDataSize ( (unsigned int)crapPage.size(), requestID );
      setURLData ( crapPage.c_str(), requestID );
    }
    else
    {
      fseek(fp,0,SEEK_END);
      unsigned int size = ftell(fp);
      fseek(fp,0,SEEK_SET);
      char *p = (char*)malloc(size);
      fread(p,size,1,fp);
      setURLDataSize ( size, requestID );
      setURLData ( p, requestID ); 
      fclose(fp);
      free(p);
    }
  }
}


// Local Variables: ***
// mode:C++ ***
// tab-width: 8 ***
// c-basic-offset: 2 ***
// indent-tabs-mode: t ***
// End: ***
// ex: shiftwidth=2 tabstop=8