#ifndef __OBJECTS_H__
#define __OBJECTS_H__

#include "camera.h"
#include <iostream>
#include <string>
#include "TextureMngr.h"
#include "Model.h"
#include "World.h"

void RegisterClasses ( CWorld *pWorld );

class Ground : public CBaseObject
{
public:
	virtual	bool Init ( void );
	virtual void Render(bool transparent = false, bool bSelected = false, int iName = -1);
	virtual bool ShowInInterface ( void );
	virtual bool OnlyOne ( void ) { return true;}
	virtual bool ForcedAdd ( void ) { return true;}

private:
	int iTexture;
	int	iModel;
};

class Walls : public CBaseObject
{
public:
	virtual bool Init ( void );
	virtual void Render(bool transparent = false, bool bSelected = false, int iName = -1);
	virtual bool ShowInInterface ( void );
	virtual bool OnlyOne ( void ) { return true;}
	virtual bool ForcedAdd ( void ) { return true;}

private:
	int iTexture;
	int	iModel;
};

class Box : public CBaseObject
{
public:
	Box();
	Box(const Box &r);
	Box operator = (const Box &r);
	virtual void CopyFrom (CBaseObject *r);

	virtual bool Init ( void );

	virtual void Render(bool transparent = false, bool bSelected = false, int iName = -1);

	virtual void Write( std::ostream &Stream );
	virtual void Write( char *data );
	virtual bool Read( char *data );

	virtual void* GetIcon ( void );

	// to be called by the editor
private:
	int	iWallTexture;
	int iRoofTexture;

	int	iSidesModel;
	int	iTopsModel;
};

class Pyramid : public CBaseObject
{
public:
	Pyramid();
	Pyramid(const Pyramid &r);
	Pyramid operator = (const Pyramid &r);
	virtual void CopyFrom (CBaseObject *r);

	virtual bool Init ( void );

	virtual void Render(bool transparent = false, bool bSelected = false, int iName = -1);

	virtual void Write( std::ostream &Stream );
	virtual void Write( char *data );
	virtual bool Read( char *data );
	virtual void* GetIcon ( void );
private:
	int	iTexture;
	int	iModel;
};

class Teleporter : public CBaseObject
{
public:
	Teleporter();
	Teleporter(const Teleporter &r);
	Teleporter operator = (const Teleporter &r);
	virtual void CopyFrom (CBaseObject *r);

	virtual bool Init ( void );

	virtual void Render(bool transparent = false, bool bSelected = false, int iName = -1);

	virtual void Write( std::ostream &Stream );
	virtual void Write( char *data );
	virtual bool Read( char *data );
	virtual void* GetIcon ( void );

	bool SetItemInfo ( char  *szName, float fData );
	bool GetItemInfo ( char  *szName, float &fData );

	// need a border so we need to do this
	virtual void* GetEditInterface ( void *pParam );
	virtual void CloseEditInterface ( void );
	
	virtual void UpdateEditInfo ( void );
	virtual void ApplyEdit ( void );
	virtual void RevertEdit ( void );

protected:
	float	border;
	int		iTexture;	
	int		iFieldTexture;	
	int		iFieldModel;
	int		iFrameModel;
};

class Base : public CBaseObject
{
public:
	Base();
	Base(const Base &b);
	Base operator = (const Base &b);
	virtual void CopyFrom (CBaseObject *r);

	virtual bool Init ( void );

	virtual void Render(bool transparent = false, bool bSelected = false, int iName = -1);

	virtual void Write( std::ostream &Stream );
	virtual void Write( char *data );
	virtual bool Read( char *data );
	virtual void* GetIcon ( void );

	bool SetItemInfo ( char  *szName, int iData );
	bool GetItemInfo ( char  *szName, int &iData );

	// need a team so we need to do this
	virtual void* GetEditInterface ( void *pParam );
	virtual void CloseEditInterface ( void );
	
	virtual void UpdateEditInfo ( void );
	virtual void ApplyEdit ( void );
	virtual void RevertEdit ( void );

protected:
	int color;
	int	iBaseModel;
	int	iWallsModel;
};

// just cus this little guy needs it
class Link : public CBaseObject
{
public:
	Link();
	Link(const Link &r);
	Link operator = (const Link &r);
	virtual void CopyFrom (CBaseObject *r);

	virtual bool Init ( void );

	virtual void Render(bool transparent = false, bool bSelected = false, int iName = -1);

	virtual void Write( std::ostream &Stream );
	virtual void Write( char *data );
	virtual	bool Read( char *data );
	virtual void* GetIcon ( void );

	bool SetItemInfo ( char  *szName, int iData );
	bool GetItemInfo ( char  *szName, int &iData );

	// custom interface so.....
	virtual void* GetEditInterface ( void *pParam );
	virtual void CloseEditInterface ( void );
	
	virtual void UpdateEditInfo ( void );
	virtual void ApplyEdit ( void );
	virtual void RevertEdit ( void );

protected:
	int				from_side;
	int				to_side;
	int				iLinkModel;
};

class GroundFactory : public CBaseObjectFactory
{
public:
	virtual CBaseObject* New( void ) { return new Ground; }
};

class WallsFactory : public CBaseObjectFactory
{
public:
	virtual CBaseObject* New( void ) { return new Walls; }
};

class BoxFactory : public CBaseObjectFactory
{
public:
	virtual CBaseObject* New( void ) { return new Box; }
};

class PyramidFactory : public CBaseObjectFactory
{
public:
	virtual CBaseObject* New( void ) { return new Pyramid; }
};

class BaseFactory : public CBaseObjectFactory
{
public:
	virtual CBaseObject* New( void ) { return new Base; }
};

class TeleporterFactory : public CBaseObjectFactory
{
public:
	virtual CBaseObject* New( void ) { return new Teleporter; }
};

class LinkFactory : public CBaseObjectFactory
{
public:
	virtual CBaseObject* New( void ) { return new Link; }
};

#endif