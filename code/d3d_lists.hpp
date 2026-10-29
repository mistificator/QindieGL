/***************************************************************************
* Copyright (C) 2011-2016, Crystice Softworks.
*
* This file is part of QindieGL source code.
* Please note that QindieGL is not driver, it's emulator.
*
* QindieGL source code is free software; you can redistribute it and/or
* modify it under the terms of the GNU General Public License as
* published by the Free Software Foundation; either version 2 of
* the License, or (at your option) any later version.
*
* QindieGL source code is distributed in the hope that it will be
* useful, but WITHOUT ANY WARRANTY; without even the implied
* warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
* See the GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA
***************************************************************************/
#ifndef QINDIEGL_D3D_LISTS_H
#define QINDIEGL_D3D_LISTS_H

#define IMPL_MAX_LIST_NESTING 64

extern bool g_bCompilingList;
extern bool g_bExecuteList;

inline bool D3DLists_IsCompiling() { return g_bCompilingList; }
inline bool D3DLists_IsExecuteMode() { return g_bExecuteList; }

// Helper macro to intercept OpenGL commands during glNewList..glEndList
#define D3DLISTS_RECORD( record_call ) \
	do { \
		if ( D3DLists_IsCompiling() ) { \
			record_call; \
			if ( !D3DLists_IsExecuteMode() ) \
				return; \
		} \
	} while ( 0 )

// Lifecycle & state queries
extern void D3DLists_Init();
extern void D3DLists_Cleanup();
extern GLuint D3DLists_GetListBase();
extern GLuint D3DLists_GetListIndex();
extern GLenum D3DLists_GetListMode();
extern void D3DLists_PushAttrib();
extern void D3DLists_PopAttrib();

// Immediate mode recording
extern void D3DLists_RecordBegin( GLenum mode );
extern void D3DLists_RecordEnd();
extern void D3DLists_RecordVertex3( GLfloat x, GLfloat y, GLfloat z );
extern void D3DLists_RecordVertex4( GLfloat x, GLfloat y, GLfloat z, GLfloat w );
extern void D3DLists_RecordColor( DWORD color );
extern void D3DLists_RecordColor2( DWORD color2Rgb );
extern void D3DLists_RecordNormal( GLfloat nx, GLfloat ny, GLfloat nz );
extern void D3DLists_RecordTexCoord( GLenum target, GLfloat s, GLfloat t, GLfloat r, GLfloat q, int num );
extern void D3DLists_RecordFogCoord( GLfloat coord );

// Matrix operations recording
extern void D3DLists_RecordMatrixMode( GLenum mode );
extern void D3DLists_RecordLoadIdentity();
extern void D3DLists_RecordLoadMatrix( const GLfloat *m );
extern void D3DLists_RecordMultMatrix( const GLfloat *m );
extern void D3DLists_RecordLoadTransposeMatrix( const GLfloat *m );
extern void D3DLists_RecordMultTransposeMatrix( const GLfloat *m );
extern void D3DLists_RecordFrustum( GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble zNear, GLdouble zFar );
extern void D3DLists_RecordOrtho( GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble zNear, GLdouble zFar );
extern void D3DLists_RecordPushMatrix();
extern void D3DLists_RecordPopMatrix();
extern void D3DLists_RecordRotate( GLfloat angle, GLfloat x, GLfloat y, GLfloat z );
extern void D3DLists_RecordScale( GLfloat x, GLfloat y, GLfloat z );
extern void D3DLists_RecordTranslate( GLfloat x, GLfloat y, GLfloat z );

// State, blending, lighting, materials, textures, stencil, misc recording
extern void D3DLists_RecordEnable( GLenum cap );
extern void D3DLists_RecordDisable( GLenum cap );
extern void D3DLists_RecordHint( GLenum target, GLenum mode );
extern void D3DLists_RecordPushAttrib( GLbitfield mask );
extern void D3DLists_RecordPopAttrib();
extern void D3DLists_RecordAlphaFunc( GLenum func, GLclampf ref );
extern void D3DLists_RecordBlendFunc( GLenum sfactor, GLenum dfactor );
extern void D3DLists_RecordBlendColor( GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha );
extern void D3DLists_RecordBlendEquation( GLenum mode );
extern void D3DLists_RecordFogfv( GLenum pname, const GLfloat *params );
extern void D3DLists_RecordClipPlane( GLenum plane, const GLdouble *equation );
extern void D3DLists_RecordLightModelf( GLenum pname, GLfloat param );
extern void D3DLists_RecordLightModelfv( GLenum pname, const GLfloat *params );
extern void D3DLists_RecordLightf( GLenum light, GLenum pname, GLfloat param );
extern void D3DLists_RecordLightfv( GLenum light, GLenum pname, const GLfloat *params );
extern void D3DLists_RecordColorMaterial( GLenum face, GLenum mode );
extern void D3DLists_RecordMaterialf( GLenum face, GLenum pname, GLfloat param );
extern void D3DLists_RecordMaterialfv( GLenum face, GLenum pname, const GLfloat *params );
extern void D3DLists_RecordClear( GLbitfield mask );
extern void D3DLists_RecordClearColor( GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha );
extern void D3DLists_RecordClearDepth( GLclampd depth );
extern void D3DLists_RecordClearStencil( GLint s );
extern void D3DLists_RecordColorMask( GLboolean red, GLboolean green, GLboolean blue, GLboolean alpha );
extern void D3DLists_RecordCullFace( GLenum mode );
extern void D3DLists_RecordFrontFace( GLenum mode );
extern void D3DLists_RecordDepthFunc( GLenum func );
extern void D3DLists_RecordDepthMask( GLboolean flag );
extern void D3DLists_RecordDepthRange( GLclampd zNear, GLclampd zFar );
extern void D3DLists_RecordPolygonMode( GLenum face, GLenum mode );
extern void D3DLists_RecordPolygonOffset( GLfloat factor, GLfloat units );
extern void D3DLists_RecordShadeModel( GLenum mode );
extern void D3DLists_RecordPointSize( GLfloat size );
extern void D3DLists_RecordViewport( GLint x, GLint y, GLsizei width, GLsizei height );
extern void D3DLists_RecordScissor( GLint x, GLint y, GLsizei width, GLsizei height );
extern void D3DLists_RecordStencilFunc( GLenum func, GLint ref, GLuint mask );
extern void D3DLists_RecordStencilMask( GLuint mask );
extern void D3DLists_RecordStencilOp( GLenum fail, GLenum zfail, GLenum zpass );
extern void D3DLists_RecordActiveStencilFace( GLenum face );
extern void D3DLists_RecordBindTexture( GLenum target, GLuint texture );
extern void D3DLists_RecordActiveTexture( GLenum texture );
extern void D3DLists_RecordSelectTexture( GLenum texture );
extern void D3DLists_RecordPrioritizeTextures( GLsizei n, const GLuint *textures, const GLclampf *priorities );
extern void D3DLists_RecordTexEnvf( GLenum target, GLenum pname, GLfloat param );
extern void D3DLists_RecordTexEnvi( GLenum target, GLenum pname, GLint param );
extern void D3DLists_RecordTexEnvfv( GLenum target, GLenum pname, const GLfloat *params );
extern void D3DLists_RecordTexEnviv( GLenum target, GLenum pname, const GLint *params );
extern void D3DLists_RecordTexParameterf( GLenum target, GLenum pname, GLfloat param );
extern void D3DLists_RecordTexParameteri( GLenum target, GLenum pname, GLint param );
extern void D3DLists_RecordTexParameterfv( GLenum target, GLenum pname, const GLfloat *params );
extern void D3DLists_RecordTexParameteriv( GLenum target, GLenum pname, const GLint *params );
extern void D3DLists_RecordTexGenf( GLenum coord, GLenum pname, GLfloat param );
extern void D3DLists_RecordTexGeni( GLenum coord, GLenum pname, GLint param );
extern void D3DLists_RecordTexGend( GLenum coord, GLenum pname, GLdouble param );
extern void D3DLists_RecordTexGenfv( GLenum coord, GLenum pname, const GLfloat *params );
extern void D3DLists_RecordTexGeniv( GLenum coord, GLenum pname, const GLint *params );
extern void D3DLists_RecordTexGendv( GLenum coord, GLenum pname, const GLdouble *params );
extern void D3DLists_RecordPixelTransferf( GLenum pname, GLfloat param );
extern void D3DLists_RecordPixelTransferi( GLenum pname, GLint param );

#endif //QINDIEGL_D3D_LISTS_H
