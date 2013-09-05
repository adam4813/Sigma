#pragma  once

class IOpSys {
public:
	IOpSys() { }
	virtual ~IOpSys() { }

	virtual void* CreateGraphicsWindow() = 0;

	virtual bool MessageLoop() = 0;

	virtual bool SetupTimer() = 0;

	virtual double GetDeltaTime() = 0;

	virtual bool KeyDown(int key, bool focused = false) = 0;

	virtual bool KeyUp( int key, bool focused = false ) = 0;

	virtual void Present() = 0;


};