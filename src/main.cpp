#include <psp2/kernel/threadmgr.h>
#include <psp2/kernel/processmgr.h>
#include <psp2/sysmodule.h>
#include "debugnet.h"
#include "Context.hh"
#include "ShaderFactory.hh"
#include "Scene.hh"

#define ip_server "192.168.1.24"
#define port_server 18194

class Bounce
{
public :
  Bounce() :
    _scene(_context, _shaderFactory)
  { }
  
  void init()
  {
    memset(&_pad, 0, sizeof(_pad));
    sceCtrlSetSamplingMode(SCE_CTRL_MODE_ANALOG);
    _scene.setup();
  }
  
  void run()
  {
    sceKernelGetProcessTime(&_sysClock);
    float lastT = (_sysClock / 1000000.0f);

    while (true)
      {
	sceKernelGetProcessTime(&_sysClock);
	float totalT = (_sysClock / 1000000.0f);

	sceCtrlPeekBufferPositive(0, &_pad, 1);   
	if (_pad.buttons & SCE_CTRL_START)
	  break;
	
	_scene.update(_pad, totalT, totalT - lastT);
	_context.startDrawing();
	_scene.draw();
	_context.endDrawing();
	_context.swapBuffers();
	
	lastT = totalT;	
      }
  }
  
private:
  Context		_context;
  ShaderFactory		_shaderFactory;
  Scene			_scene;
  SceCtrlData		_pad;
  SceKernelSysClock	_sysClock;
};

int	main(int argc, char *argv[])
{
  sceSysmoduleLoadModule(SCE_SYSMODULE_NET);
  debugNetInit((char*)ip_server, port_server, DEBUG);

  debugNetPrintf(DEBUG, "Bounce !");

  try
    {
      Bounce bounce;

      bounce.init();
      bounce.run();
    }
  catch (const std::runtime_error& e)
    {
      debugNetPrintf(ERROR, e.what());
    }
    
  sceKernelExitProcess(0);
  return 0;
}
