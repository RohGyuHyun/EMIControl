#include "stdafx.h"
#include <io.h>

#include "recipe.h"
#include "string_mp.h"

#include <assert.h>
#include <stdio.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace RECIPE_NAMESPACE;

//////////////////////////////////////////////////////////////////////
// Global Variables..
recipe	g_recipe;

recipe::recipe()
{
}

const enum 
{
	MAX_CHARS	= 255,
	MAX_ITEMS	= 20,
};


//#######################################################################################
//#######################################################################################
//##########################    Read / Write RECIPE    ##################################
//#######################################################################################
//#######################################################################################

BOOL recipe::ReadRecipe( const char *szFileName )
{
	FILE* fp;
	if ( (fp=fopen( szFileName, "rt" )) == NULL )	return FALSE;
	
	listGenRecipes.clear();
	listCmpMode0Recipes.clear();
	listCmpMode1Recipes.clear();
	listMode0Recipes.clear();
	listMode1Recipes.clear();
	listMode2Recipes.clear();
	listMode3Recipes.clear();
	listMode4BoundRcps.clear();
	listMode4Recipes.clear();
	listMode5Recipes.clear();
	listCompRecipes.clear();

	char line[MAX_CHARS], line_temp[MAX_CHARS];
	char *items[MAX_ITEMS];
	int nitems;

	while ( fgets( line, sizeof(line), fp ) != NULL )
	{
		if ( line[0] == ' ' || line[0] == '#' || line[0] == '\r' || line[0] == '\n' ) 
			continue;

		memcpy( line_temp, line, sizeof(line_temp) );
		nitems = parse_items( items, MAX_ITEMS, line_temp, "=," );
		//nitems = parse_items(items, MAX_ITEMS, line_temp, " ");

		if ( strcmp( items[0], "[GENERAL RECIPES]" ) == 0)
		{
			GENRECIPE genrecipe;
			genrecipe.d_EncoderResolution = (double)atof(items[1]);
			//genrecipe.d_Mode3BasePitch = (double)atof(items[2]);
			listGenRecipes.push_back(genrecipe);
			continue;
		}

		if ( strcmp( items[0], "[CMPMODE0 RECIPES]" ) == 0)
		{
			CMPMODE0RECIPE cmpmode0recipe;
			cmpmode0recipe.n_ID = atoi(items[1]);
			cmpmode0recipe.d_Freq = (double)atof(items[2]);
			cmpmode0recipe.d_Period = (double)atof(items[3]);
			cmpmode0recipe.d_PulseWidth = (double)atof(items[4]);
			listCmpMode0Recipes.push_back(cmpmode0recipe);
			continue;
		}

		if ( strcmp( items[0], "[CMPMODE1 RECIPES]" ) == 0)
		{
			CMPMODE1RECIPE cmpmode1recipe;
			cmpmode1recipe.n_ID = atoi(items[1]);
			cmpmode1recipe.d_StartPos = (double)atof(items[2]);
			cmpmode1recipe.d_EndPos = (double)atof(items[3]);
			cmpmode1recipe.d_Pitch = (double)atof(items[4]);
			cmpmode1recipe.d_PulseWidth = (double)atof(items[5]);
			cmpmode1recipe.d_Delay = (double)atof(items[6]);
			listCmpMode1Recipes.push_back(cmpmode1recipe);
			continue;
		}

		if ( strcmp( items[0], "[MODE0 RECIPES]" ) == 0)
		{
			MODE0RECIPE mode0recipe;
			mode0recipe.d_Freq = (double)atof(items[1]);
			mode0recipe.d_Period = (double)atof(items[2]);
			mode0recipe.d_PulseWidth = (double)atof(items[3]);
			listMode0Recipes.push_back(mode0recipe);
			continue;
		}

		if ( strcmp( items[0], "[MODE1 RECIPES]" ) == 0)
		{
			MODE1RECIPE mode1recipe;
			mode1recipe.d_StartPos = (double)atof(items[1]);
			mode1recipe.d_EndPos = (double)atof(items[2]);
			mode1recipe.d_Pitch = (double)atof(items[3]);
			mode1recipe.d_PulseWidth = (double)atof(items[4]);
			mode1recipe.d_Delay = (double)atof(items[5]);
			listMode1Recipes.push_back(mode1recipe);
			continue;
		}

		if ( strcmp( items[0], "[MODE2 RECIPES]" ) == 0)
		{
			MODE2RECIPE mode2recipe;
			mode2recipe.d_Pitch = (double)atof(items[1]);
			mode2recipe.d_PulseWidth = (double)atof(items[2]);
			mode2recipe.d_Delay = (double)atof(items[3]);
			listMode2Recipes.push_back(mode2recipe);
			continue;
		}

		if ( strcmp( items[0], "[MODE3 RECIPES]" ) == 0)
		{
			MODE3RECIPE mode3recipe;
			mode3recipe.n_ID = atoi(items[1]);
			mode3recipe.d_Speed = (double)atof(items[2]);
			mode3recipe.d_Freq = (double)atof(items[3]);
			mode3recipe.d_PulseWidth = (double)atof(items[4]);
			listMode3Recipes.push_back(mode3recipe);
			continue;
		}

		if ( strcmp( items[0], "[MODE4 BOUNDRCPS]" ) == 0)
		{
			MODE4BOUNDRCP mode4boundrcp;
			mode4boundrcp.n_ID = atoi(items[1]);
			mode4boundrcp.d_xBound = (double)atof(items[2]);
			mode4boundrcp.d_yBound = (double)atof(items[3]);
			listMode4BoundRcps.push_back(mode4boundrcp);
			continue;
		}

		if ( strcmp( items[0], "[MODE4 RECIPES]" ) == 0)
		{
			MODE4RECIPE mode4recipe;
			mode4recipe.n_ID = atoi(items[1]);
			mode4recipe.d_Freq = (double)atof(items[2]);
			mode4recipe.d_PulseWidth = (double)atof(items[3]);
			listMode4Recipes.push_back(mode4recipe);
			continue;
		}

		if ( strcmp( items[0], "[MODE5 RECIPES]" ) == 0)
		{
			MODE5RECIPE mode5recipe;
			mode5recipe.n_ID = atoi(items[1]);
			mode5recipe.n_dir = atoi(items[2]);
			mode5recipe.n_lineend = atoi(items[3]);
			mode5recipe.d_Pos = (double)atof(items[4]);
			mode5recipe.d_PulseWidth = (double)atof(items[5]);
			listMode5Recipes.push_back(mode5recipe);
			continue;
		}

		if ( strcmp( items[0], "[COMP RECIPES]" ) == 0)
		{
			COMPRECIPE comprecipe;
			comprecipe.n_ID = atoi(items[1]);
			comprecipe.n_BoundCount = atoi(items[2]);
			comprecipe.n_CompCount = atoi(items[3]);
			listCompRecipes.push_back(comprecipe);
			continue;
		}
	}

	fclose( fp );

	return TRUE;
}

BOOL recipe::WriteRecipe( const char *szFileName )
{
	FILE* fp;
	if ( (fp=fopen( szFileName, "wt" )) == NULL )	return FALSE;

	SYSTEMTIME time;
	::GetLocalTime( &time );

	fprintf( fp, "# %s\n", szFileName );
	fprintf( fp, "# %04d/%02d/%02d %02d:%02d:%02d\n",time.wYear,time.wMonth,time.wDay,time.wHour,time.wMinute,time.wSecond );
	fprintf( fp, "# Generated by Machine\n" );
	fprintf( fp, "# Copyright DaeKhon Corporation\n\n" );

	fprintf( fp, "# General Information #####################################################################\n" );
	fprintf( fp, "# [GENERAL RECIPES] = encoder(pulse) resolution(um), Not Used,\n");

	GENRECIPES::const_iterator G;
	for(G=listGenRecipes.begin();G!=listGenRecipes.end();G++)
	{
		fprintf(fp,"[GENERAL RECIPES]=%.4f,\n",G->d_EncoderResolution);
	}

	fprintf( fp, "\n");
	fprintf( fp, "# Compact Mode0 Information ###############################################################\n" );
	fprintf( fp, "# [CMPMODE0 RECIPES] = channel id, frequency(kHz), period(us), pulse width(us),\n\n");

	CMPMODE0RECIPES::const_iterator B;
	for(B=listCmpMode0Recipes.begin();B!=listCmpMode0Recipes.end();B++)
	{
		fprintf(fp,"[CMPMODE0 RECIPES]=%d,%.4f,%.2f,%.2f,\n",B->n_ID,B->d_Freq,B->d_Period,B->d_PulseWidth);
	}

	fprintf( fp, "\n");
	fprintf( fp, "# Compact Mode1 Information ###############################################################\n" );
	fprintf( fp, "# [CMPMODE1 RECIPES] = channel id, start position(mm), end position(mm), pitch(um), pulse width(us), delay(us),\n\n");

	CMPMODE1RECIPES::const_iterator K;
	for(K=listCmpMode1Recipes.begin();K!=listCmpMode1Recipes.end();K++)
	{
		fprintf(fp,"[CMPMODE1 RECIPES]=%d,%.4f,%.4f,%.3f,%.2f,%.2f,\n",K->n_ID,K->d_StartPos,K->d_EndPos,K->d_Pitch,K->d_PulseWidth,K->d_Delay);
	}

	fclose( fp );

	return TRUE;
}

int recipe::parse_items( char *items[], int nr_items, char *string, const char *separators )
{
	char *token;
	int item_count=0;
	
	token = strtok( string, separators );
	while ( token != NULL ) {
		items[item_count++]=token;	
		token = strtok( NULL, separators );
	}
	
	assert( item_count>0 );
	if ( (token = strchr(items[item_count-1], '\r' )) != NULL ) {
		*token='\0';
	}
	if ( (token = strchr(items[item_count-1], '\n' )) != NULL ) {
		*token='\0';
	}

	return item_count;
}


//#######################################################################################
//#######################################################################################
//######################    Copy List/Temp to Temp/List    ##############################
//#######################################################################################
//#######################################################################################

BOOL recipe::CopyListToTemp()
{
	listTempGenRecipes.clear();
	listTempCmpMode0Recipes.clear();
	listTempCmpMode1Recipes.clear();
	listTempMode0Recipes.clear();
	listTempMode1Recipes.clear();
	listTempMode2Recipes.clear();
	listTempMode3Recipes.clear();
	listTempMode4BoundRcps.clear();
	listTempMode4Recipes.clear();
	listTempMode5Recipes.clear();
	listTempCompRecipes.clear();

	GENRECIPES::const_iterator G;
	for( G=listGenRecipes.begin() ; G!=listGenRecipes.end() ; G++)
	{
		GENRECIPE gendata;
		gendata.d_EncoderResolution = G->d_EncoderResolution;
		gendata.d_Mode3BasePitch = G->d_Mode3BasePitch;
		listTempGenRecipes.push_back(gendata);
	}

	CMPMODE0RECIPES::const_iterator D;
	for( D=listCmpMode0Recipes.begin() ; D!=listCmpMode0Recipes.end() ; D++)
	{
		CMPMODE0RECIPE cmpmode0data;
		cmpmode0data.n_ID = D->n_ID;
		cmpmode0data.d_Freq = D->d_Freq;
		cmpmode0data.d_Period = D->d_Period;
		cmpmode0data.d_PulseWidth = D->d_PulseWidth;
		listTempCmpMode0Recipes.push_back(cmpmode0data);
	}

	CMPMODE1RECIPES::const_iterator E;
	for( E=listCmpMode1Recipes.begin() ; E!=listCmpMode1Recipes.end() ; E++)
	{
		CMPMODE1RECIPE cmpmode1data;
		cmpmode1data.n_ID = E->n_ID;
		cmpmode1data.d_StartPos = E->d_StartPos;
		cmpmode1data.d_EndPos = E->d_EndPos;
		cmpmode1data.d_Pitch = E->d_Pitch;
		cmpmode1data.d_PulseWidth = E->d_PulseWidth;
		cmpmode1data.d_Delay = E->d_Delay;
		listTempCmpMode1Recipes.push_back(cmpmode1data);
	}

	MODE0RECIPES::const_iterator H;
	for( H=listMode0Recipes.begin() ; H!=listMode0Recipes.end() ; H++)
	{
		MODE0RECIPE mode0data;
		mode0data.d_Freq = H->d_Freq;
		mode0data.d_Period = H->d_Period;
		mode0data.d_PulseWidth = H->d_PulseWidth;
		listTempMode0Recipes.push_back(mode0data);
	}

	MODE1RECIPES::const_iterator I;
	for( I=listMode1Recipes.begin() ; I!=listMode1Recipes.end() ; I++)
	{
		MODE1RECIPE mode1data;
		mode1data.d_StartPos = I->d_StartPos;
		mode1data.d_EndPos = I->d_EndPos;
		mode1data.d_Pitch = I->d_Pitch;
		mode1data.d_PulseWidth = I->d_PulseWidth;
		mode1data.d_Delay = I->d_Delay;
		listTempMode1Recipes.push_back(mode1data);
	}

	MODE2RECIPES::const_iterator J;
	for( J=listMode2Recipes.begin() ; J!=listMode2Recipes.end() ; J++)
	{
		MODE2RECIPE mode2data;
		mode2data.d_Pitch = J->d_Pitch;
		mode2data.d_PulseWidth = J->d_PulseWidth;
		mode2data.d_Delay = J->d_Delay;
		listTempMode2Recipes.push_back(mode2data);
	}

	MODE3RECIPES::const_iterator K;
	for ( K=listMode3Recipes.begin() ; K!=listMode3Recipes.end() ; K++ )
	{
		MODE3RECIPE mode3data;
		mode3data.n_ID = K->n_ID;
		mode3data.d_Speed = K->d_Speed;
		mode3data.d_Freq = K->d_Freq;
		mode3data.d_PulseWidth = K->d_PulseWidth;
		listTempMode3Recipes.push_back(mode3data);
	}

	MODE4BOUNDRCPS::const_iterator M;
	for ( M=listMode4BoundRcps.begin() ; M!=listMode4BoundRcps.end() ; M++ )
	{
		MODE4BOUNDRCP mode4bounddata;
		mode4bounddata.n_ID = M->n_ID;
		mode4bounddata.d_xBound = M->d_xBound;
		mode4bounddata.d_yBound = M->d_yBound;
		listTempMode4BoundRcps.push_back(mode4bounddata);
	}

	MODE4RECIPES::const_iterator N;
	for ( N=listMode4Recipes.begin() ; N!=listMode4Recipes.end() ; N++ )
	{
		MODE4RECIPE mode4data;
		mode4data.n_ID = N->n_ID;
		mode4data.d_Freq = N->d_Freq;
		mode4data.d_PulseWidth = N->d_PulseWidth;
		listTempMode4Recipes.push_back(mode4data);
	}

	MODE5RECIPES::const_iterator P;
	for ( P=listMode5Recipes.begin() ; P!=listMode5Recipes.end() ; P++ )
	{
		MODE5RECIPE mode5data;
		mode5data.n_ID = P->n_ID;
		mode5data.n_dir = P->n_dir;
		mode5data.n_lineend = P->n_lineend;
		mode5data.d_Pos = P->d_Pos;
		mode5data.d_PulseWidth = P->d_PulseWidth;
		listTempMode5Recipes.push_back(mode5data);
	}

	COMPRECIPES::const_iterator X;
	for ( X=listCompRecipes.begin() ; X!=listCompRecipes.end() ; X++ )
	{
		COMPRECIPE compdata;
		compdata.n_ID = X->n_ID;
		compdata.n_BoundCount = X->n_BoundCount;
		compdata.n_CompCount = X->n_CompCount;
		listTempCompRecipes.push_back(compdata);
	}

	return TRUE;
}

BOOL recipe::CopyTempToList()
{
	listGenRecipes.clear();
	listCmpMode0Recipes.clear();
	listCmpMode1Recipes.clear();
	listMode0Recipes.clear();
	listMode1Recipes.clear();
	listMode2Recipes.clear();
	listMode3Recipes.clear();
	listMode4BoundRcps.clear();
	listMode4Recipes.clear();
	listMode5Recipes.clear();
	listCompRecipes.clear();

	TEMPGENRECIPES::const_iterator G;
	for( G=listTempGenRecipes.begin() ; G!=listTempGenRecipes.end() ; G++)
	{
		GENRECIPE gendata;
		gendata.d_EncoderResolution = G->d_EncoderResolution;
		gendata.d_Mode3BasePitch = G->d_Mode3BasePitch;
		listGenRecipes.push_back(gendata);
	}

	TEMPCMPMODE0RECIPES::const_iterator D;
	for( D=listTempCmpMode0Recipes.begin() ; D!=listTempCmpMode0Recipes.end() ; D++)
	{
		CMPMODE0RECIPE cmpmode0data;
		cmpmode0data.n_ID = D->n_ID;
		cmpmode0data.d_Freq = D->d_Freq;
		cmpmode0data.d_Period = D->d_Period;
		cmpmode0data.d_PulseWidth = D->d_PulseWidth;
		listCmpMode0Recipes.push_back(cmpmode0data);
	}

	TEMPCMPMODE1RECIPES::const_iterator E;
	for( E=listTempCmpMode1Recipes.begin() ; E!=listTempCmpMode1Recipes.end() ; E++)
	{
		CMPMODE1RECIPE cmpmode1data;
		cmpmode1data.n_ID = E->n_ID;
		cmpmode1data.d_StartPos = E->d_StartPos;
		cmpmode1data.d_EndPos = E->d_EndPos;
		cmpmode1data.d_Pitch = E->d_Pitch;
		cmpmode1data.d_PulseWidth = E->d_PulseWidth;
		cmpmode1data.d_Delay = E->d_Delay;
		listCmpMode1Recipes.push_back(cmpmode1data);
	}
/*
	TEMPMODE0RECIPES::const_iterator H;
	for( H=listTempMode0Recipes.begin() ; H!=listTempMode0Recipes.end() ; H++)
	{
		MODE0RECIPE mode0data;
		mode0data.d_Freq = H->d_Freq;
		mode0data.d_Period = H->d_Period;
		mode0data.d_PulseWidth = H->d_PulseWidth;
		listMode0Recipes.push_back(mode0data);
	}

	TEMPMODE1RECIPES::const_iterator I;
	for( I=listTempMode1Recipes.begin() ; I!=listTempMode1Recipes.end() ; I++)
	{
		MODE1RECIPE mode1data;
		mode1data.d_StartPos = I->d_StartPos;
		mode1data.d_EndPos = I->d_EndPos;
		mode1data.d_Pitch = I->d_Pitch;
		mode1data.d_PulseWidth = I->d_PulseWidth;
		mode1data.d_Delay = I->d_Delay;
		listMode1Recipes.push_back(mode1data);
	}

	TEMPMODE2RECIPES::const_iterator J;
	for( J=listTempMode2Recipes.begin() ; J!=listTempMode2Recipes.end() ; J++)
	{
		MODE2RECIPE mode2data;
		mode2data.d_Pitch = J->d_Pitch;
		mode2data.d_PulseWidth = J->d_PulseWidth;
		mode2data.d_Delay = J->d_Delay;
		listMode2Recipes.push_back(mode2data);
	}

	TEMPMODE3RECIPES::const_iterator K;
	for ( K=listTempMode3Recipes.begin() ; K!=listTempMode3Recipes.end() ; K++ )
	{
		MODE3RECIPE mode3data;
		mode3data.n_ID = K->n_ID;
		mode3data.d_Speed = K->d_Speed;
		mode3data.d_Freq = K->d_Freq;
		mode3data.d_PulseWidth = K->d_PulseWidth;
		listMode3Recipes.push_back(mode3data);
	}

	TEMPMODE4BOUNDRCPS::const_iterator M;
	for ( M=listTempMode4BoundRcps.begin() ; M!=listTempMode4BoundRcps.end() ; M++ )
	{
		MODE4BOUNDRCP mode4bounddata;
		mode4bounddata.n_ID = M->n_ID;
		mode4bounddata.d_xBound = M->d_xBound;
		mode4bounddata.d_yBound = M->d_yBound;
		listMode4BoundRcps.push_back(mode4bounddata);
	}

	TEMPMODE4RECIPES::const_iterator N;
	for ( N=listTempMode4Recipes.begin() ; N!=listTempMode4Recipes.end() ; N++ )
	{
		MODE4RECIPE mode4data;
		mode4data.n_ID = N->n_ID;
		mode4data.d_Freq = N->d_Freq;
		mode4data.d_PulseWidth = N->d_PulseWidth;
		listMode4Recipes.push_back(mode4data);
	}

	TEMPMODE5RECIPES::const_iterator P;
	for ( P=listTempMode5Recipes.begin() ; P!=listTempMode5Recipes.end() ; P++ )
	{
		MODE5RECIPE mode5data;
		mode5data.n_ID = P->n_ID;
		mode5data.n_dir = P->n_dir;
		mode5data.n_lineend = P->n_lineend;
		mode5data.d_Pos = P->d_Pos;
		mode5data.d_PulseWidth = P->d_PulseWidth;
		listMode5Recipes.push_back(mode5data);
	}

	TEMPCOMPRECIPES::const_iterator X;
	for ( X=listTempCompRecipes.begin() ; X!=listTempCompRecipes.end() ; X++ )
	{
		COMPRECIPE compdata;
		compdata.n_ID = X->n_ID;
		compdata.n_BoundCount = X->n_BoundCount;
		compdata.n_CompCount = X->n_CompCount;
		listCompRecipes.push_back(compdata);
	}
*/
	return TRUE;
}

//#######################################################################################
//#######################################################################################
//######################   Get Recipe Files from a folder  ##############################
//#######################################################################################
//#######################################################################################

BOOL recipe::ReadRecipeFiles()
{
	m_recipefiles.clear();

	string recipe_path;
	//recipe_path = "C:\\VCTest\\recipe\\";
	recipe_path = "Recipe\\";
	read_recipe_list( recipe_path.data(), &m_recipefiles );

	return TRUE;
}

int recipe::read_recipe_list( const char *path, RCPFILE_LIST *recipes )
{
	read_filelist( path, "rcp", recipes );
	return recipes->size();
}

int	recipe::read_filelist( const char *path, const char *ext, RCPFILE_LIST *flist )
{
	long handle;
	struct _finddata_t fl;
	string filespec, fname;
	filespec = string(path) + "\\*." + string(ext);

	if( (handle=_findfirst( filespec.data(), &fl)) == -1 ) return 0;

	fname = string(path) + "\\";
	fname += fl.name;
	flist->push_back( fname );

	while( _findnext( handle, &fl) != -1 ) {
		fname = string(path) + "\\";
		fname += fl.name;
		flist->push_back( fname );
	}

	_findclose(handle);

	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// dacrecipe message handlers
