
#include <windows.h>
#include <string.h>

#if !defined(AFX_DACRECIPE_H__8F0F2196_D06B_4BEE_BE68_E12CD92D8489__INCLUDED_)
#define AFX_DACRECIPE_H__8F0F2196_D06B_4BEE_BE68_E12CD92D8489__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#pragma warning (disable:4786)
#include <list>

// #define ENC_RESOLUTION 0.002
// #define TRIG_OFFSET 350.0

// recipe.h : header file
//

using namespace std;

namespace RECIPE_NAMESPACE
{
	typedef struct tagGENRECIPE
	{
		double	d_EncoderResolution;	//Encoder resolution, um
		double	d_Mode3BasePitch;			//Base vector distance for velocity table, um
	} GENRECIPE;

	typedef struct tagCMPMODE0RECIPE
	{
		UINT	n_ID;					//channel id
		double	d_Freq;					//kHz
		double	d_Period;				//us
		double	d_PulseWidth;			//us
	} CMPMODE0RECIPE;

	typedef struct tagCMPMODE1RECIPE
	{
		UINT	n_ID;					//channel id
		double	d_StartPos;				//mm
		double	d_EndPos;				//mm
		double	d_Pitch;				//um
		double	d_PulseWidth;			//us
		double	d_Delay;				//us
	} CMPMODE1RECIPE;

	typedef struct tagMODE0RECIPE
	{
		double	d_Freq;					//kHz
		double	d_Period;				//us
		double	d_PulseWidth;			//us
	} MODE0RECIPE;

	typedef struct tagMODE1RECIPE
	{
		double	d_StartPos;				//mm
		double	d_EndPos;				//mm
		double	d_Pitch;				//um
		double	d_PulseWidth;			//us
		double	d_Delay;				//us
	} MODE1RECIPE;

	typedef struct tagMODE2RECIPE
	{
		double	d_Pitch;				//um
		double	d_PulseWidth;			//us
		double	d_Delay;				//us
	} MODE2RECIPE;

	typedef struct tagMODE3RECIPE
	{
		UINT	n_ID;					//Table ID
		double	d_Speed;				//Motion Speed(Vector), mm/s
		double	d_Freq;					//Frequency (Actually, it's Period in Board)
		double	d_PulseWidth;			//Pulse Width, us
	} MODE3RECIPE;

	typedef struct tagMODE4BOUNDRCP
	{
		UINT	n_ID;					//Table ID	(0~14, x,y each 15 bound)
		double	d_xBound;				//Boundary x, mm
		double	d_yBound;				//Boundary y, mm
	} MODE4BOUNDRCP;

	typedef struct tagMODE4RECIPE
	{
		UINT	n_ID;					//Table ID	(0~255)
		double	d_Freq;					//Hz
		double	d_PulseWidth;			//us
	} MODE4RECIPE;

	typedef struct tagMODE5RECIPE
	{
		UINT	n_ID;					//Table ID
		UINT	n_dir;					//Trigger Direction  '0' : decrease enccnt, '1' : increase enccnt
		UINT	n_lineend;				//Line end '0' :normal, '1' :end
		double	d_Pos;					//Trigger Position, mm
		double	d_PulseWidth;			//us
	} MODE5RECIPE;

	typedef struct tagCOMPRECIPE
	{
		UINT	n_ID;					//Table ID (0~511)
		int		n_BoundCount;			//32bit Bound Count
		int		n_CompCount;			//32bit Compensation Count 
	} COMPRECIPE;

	typedef list<GENRECIPE> GENRECIPES;
	typedef list<CMPMODE0RECIPE> CMPMODE0RECIPES;
	typedef list<CMPMODE1RECIPE> CMPMODE1RECIPES;
	typedef list<MODE0RECIPE> MODE0RECIPES;
	typedef list<MODE1RECIPE> MODE1RECIPES;
	typedef list<MODE2RECIPE> MODE2RECIPES;
	typedef list<MODE3RECIPE> MODE3RECIPES;
	typedef list<MODE4BOUNDRCP> MODE4BOUNDRCPS;
	typedef list<MODE4RECIPE> MODE4RECIPES;
	typedef list<MODE5RECIPE> MODE5RECIPES;
	typedef	list<COMPRECIPE> COMPRECIPES;
	typedef list<GENRECIPE> TEMPGENRECIPES;
	typedef list<CMPMODE0RECIPE> TEMPCMPMODE0RECIPES;
	typedef list<CMPMODE1RECIPE> TEMPCMPMODE1RECIPES;
	typedef list<MODE0RECIPE> TEMPMODE0RECIPES;
	typedef list<MODE1RECIPE> TEMPMODE1RECIPES;
	typedef list<MODE2RECIPE> TEMPMODE2RECIPES;
	typedef list<MODE3RECIPE> TEMPMODE3RECIPES;
	typedef list<MODE4BOUNDRCP> TEMPMODE4BOUNDRCPS;
	typedef list<MODE4RECIPE> TEMPMODE4RECIPES;
	typedef list<MODE5RECIPE> TEMPMODE5RECIPES;
	typedef	list<COMPRECIPE> TEMPCOMPRECIPES;

	typedef struct tagRECIPE
	{
		GENRECIPES			listGenRecipes;
		CMPMODE0RECIPES		listCmpMode0Recipes;
		CMPMODE1RECIPES		listCmpMode1Recipes;
		MODE0RECIPES		listMode0Recipes;
		MODE1RECIPES		listMode1Recipes;
		MODE2RECIPES		listMode2Recipes;
		MODE3RECIPES		listMode3Recipes;
		MODE4BOUNDRCPS		listMode4BoundRcps;
		MODE4RECIPES		listMode4Recipes;
		MODE5RECIPES		listMode5Recipes;
		COMPRECIPES			listCompRecipes;
		TEMPGENRECIPES		listTempGenRecipes;
		TEMPCMPMODE0RECIPES	listTempCmpMode0Recipes;
		TEMPCMPMODE1RECIPES	listTempCmpMode1Recipes;
		TEMPMODE0RECIPES	listTempMode0Recipes;
		TEMPMODE1RECIPES	listTempMode1Recipes;
		TEMPMODE2RECIPES	listTempMode2Recipes;
		TEMPMODE3RECIPES	listTempMode3Recipes;
		TEMPMODE4BOUNDRCPS	listTempMode4BoundRcps;
		TEMPMODE4RECIPES	listTempMode4Recipes;
		TEMPMODE5RECIPES	listTempMode5Recipes;
		TEMPCOMPRECIPES		listTempCompRecipes;
	} RECIPE;
}

/////////////////////////////////////////////////////////////////////////////
// recipe window

typedef list<string> RCPFILE_LIST;


class recipe : public RECIPE_NAMESPACE::RECIPE
{
public:
	RCPFILE_LIST m_recipefiles;

public:
	recipe();

	BOOL	ReadRecipe( const char *szFileName );
	BOOL	WriteRecipe( const char *szFileName );
	int		parse_items( char *items[], int nr_items, char *string, const char *separators );

	BOOL	CopyListToTemp();
	BOOL	CopyTempToList();

	BOOL	ReadRecipeFiles();
	int		read_recipe_list( const char *path, RCPFILE_LIST *recipes );
	int		read_filelist( const char *path, const char *ext, RCPFILE_LIST *flist);
};

extern recipe g_recipe;

#endif // !defined(AFX_DACRECIPE_H__8F0F2196_D06B_4BEE_BE68_E12CD92D8489__INCLUDED_)