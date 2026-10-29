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
#include "d3d_wrapper.hpp"
#include "d3d_global.hpp"
#include "d3d_state.hpp"
#include "d3d_utils.hpp"
#include "d3d_extension.hpp"
#include "d3d_immediate.hpp"
#include "d3d_lists.hpp"

#include <vector>

//==================================================================================
// Externals
//==================================================================================
extern OPENGL_API void WINAPI glBegin( GLenum mode );
extern OPENGL_API void WINAPI glEnd( void );
extern OPENGL_API void WINAPI glColor4ub( GLubyte red, GLubyte green, GLubyte blue, GLubyte alpha );
extern OPENGL_API void WINAPI glNormal3f( GLfloat nx, GLfloat ny, GLfloat nz );

extern OPENGL_API void WINAPI glMatrixMode( GLenum mode );
extern OPENGL_API void WINAPI glLoadIdentity( void );
extern OPENGL_API void WINAPI glLoadMatrixf( const GLfloat *m );
extern OPENGL_API void WINAPI glMultMatrixf( const GLfloat *m );
extern OPENGL_API void WINAPI glRotatef( GLfloat angle, GLfloat x, GLfloat y, GLfloat z );
extern OPENGL_API void WINAPI glScalef( GLfloat x, GLfloat y, GLfloat z );
extern OPENGL_API void WINAPI glTranslatef( GLfloat x, GLfloat y, GLfloat z );
extern OPENGL_API void WINAPI glOrtho( GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble zNear, GLdouble zFar );
extern OPENGL_API void WINAPI glFrustum( GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble zNear, GLdouble zFar );
extern OPENGL_API void WINAPI glPushMatrix( void );
extern OPENGL_API void WINAPI glPopMatrix( void );

extern OPENGL_API void WINAPI glEnable( GLenum cap );
extern OPENGL_API void WINAPI glDisable( GLenum cap );
extern OPENGL_API void WINAPI glAlphaFunc( GLenum func, GLclampf ref );
extern OPENGL_API void WINAPI glBlendFunc( GLenum sfactor, GLenum dfactor );
extern OPENGL_API void WINAPI glFogfv( GLenum pname, const GLfloat *params );
extern OPENGL_API void WINAPI glClipPlane( GLenum plane, const GLdouble *equation );
extern OPENGL_API void WINAPI glCullFace( GLenum mode );
extern OPENGL_API void WINAPI glFrontFace( GLenum mode );
extern OPENGL_API void WINAPI glDepthFunc( GLenum func );
extern OPENGL_API void WINAPI glDepthMask( GLboolean flag );
extern OPENGL_API void WINAPI glDepthRange( GLclampd zNear, GLclampd zFar );
extern OPENGL_API void WINAPI glColorMask( GLboolean red, GLboolean green, GLboolean blue, GLboolean alpha );
extern OPENGL_API void WINAPI glClear( GLbitfield mask );
extern OPENGL_API void WINAPI glClearColor( GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha );
extern OPENGL_API void WINAPI glClearDepth( GLclampd depth );
extern OPENGL_API void WINAPI glClearStencil( GLint s );
extern OPENGL_API void WINAPI glShadeModel( GLenum mode );
extern OPENGL_API void WINAPI glPolygonMode( GLenum face, GLenum mode );
extern OPENGL_API void WINAPI glPolygonOffset( GLfloat factor, GLfloat units );
extern OPENGL_API void WINAPI glPolygonStipple( const GLubyte *mask );
extern OPENGL_API void WINAPI glPointSize( GLfloat size );
extern OPENGL_API void WINAPI glLineWidth( GLfloat width );
extern OPENGL_API void WINAPI glLineStipple( GLint factor, GLushort pattern );
extern OPENGL_API void WINAPI glScissor( GLint x, GLint y, GLsizei width, GLsizei height );
extern OPENGL_API void WINAPI glViewport( GLint x, GLint y, GLsizei width, GLsizei height );
extern OPENGL_API void WINAPI glHint( GLenum target, GLenum mode );
extern OPENGL_API void WINAPI glPushAttrib( GLbitfield mask );
extern OPENGL_API void WINAPI glPopAttrib( void );

extern OPENGL_API void WINAPI glStencilFunc( GLenum func, GLint ref, GLuint mask );
extern OPENGL_API void WINAPI glStencilMask( GLuint mask );
extern OPENGL_API void WINAPI glStencilOp( GLenum fail, GLenum zfail, GLenum zpass );

extern OPENGL_API void WINAPI glLightf( GLenum light, GLenum pname, const GLfloat param );
extern OPENGL_API void WINAPI glLightfv( GLenum light, GLenum pname, const GLfloat *params );
extern OPENGL_API void WINAPI glLightModelf( GLenum pname, const GLfloat param );
extern OPENGL_API void WINAPI glLightModelfv( GLenum pname, const GLfloat *params );
extern OPENGL_API void WINAPI glMaterialf( GLenum face, GLenum pname, const GLfloat param );
extern OPENGL_API void WINAPI glMaterialfv( GLenum face, GLenum pname, const GLfloat *params );
extern OPENGL_API void WINAPI glColorMaterial( GLenum face, GLenum mode );

extern OPENGL_API void WINAPI glTexEnvf( GLenum target, GLenum pname, const GLfloat param );
extern OPENGL_API void WINAPI glTexEnvi( GLenum target, GLenum pname, const GLint param );
extern OPENGL_API void WINAPI glTexEnvfv( GLenum target, GLenum pname, const GLfloat *params );
extern OPENGL_API void WINAPI glTexEnviv( GLenum target, GLenum pname, const GLint *params );
extern OPENGL_API void WINAPI glTexParameterf( GLenum target, GLenum pname, const GLfloat param );
extern OPENGL_API void WINAPI glTexParameteri( GLenum target, GLenum pname, const GLint param );
extern OPENGL_API void WINAPI glTexParameterfv( GLenum target, GLenum pname, const GLfloat *params );
extern OPENGL_API void WINAPI glTexParameteriv( GLenum target, GLenum pname, const GLint *params );
extern OPENGL_API void WINAPI glTexGenf( GLenum coord, GLenum pname, const GLfloat param );
extern OPENGL_API void WINAPI glTexGeni( GLenum coord, GLenum pname, const GLint param );
extern OPENGL_API void WINAPI glTexGend( GLenum coord, GLenum pname, const GLdouble param );
extern OPENGL_API void WINAPI glTexGenfv( GLenum coord, GLenum pname, const GLfloat *params );
extern OPENGL_API void WINAPI glTexGeniv( GLenum coord, GLenum pname, const GLint *params );
extern OPENGL_API void WINAPI glTexGendv( GLenum coord, GLenum pname, const GLdouble *params );

extern OPENGL_API void WINAPI glRasterPos4f( GLfloat x, GLfloat y, GLfloat z, GLfloat w );
extern OPENGL_API void WINAPI glBitmap( GLsizei width, GLsizei height, GLfloat xorig, GLfloat yorig, GLfloat xmove, GLfloat ymove, const GLubyte *bitmap );

extern OPENGL_API void WINAPI glPixelTransferf( GLenum pname, GLfloat param );
extern OPENGL_API void WINAPI glPixelTransferi( GLenum pname, GLint param );

extern OPENGL_API void WINAPI glListBase( GLuint base );
extern OPENGL_API void WINAPI glNewList( GLuint list, GLenum mode );
extern OPENGL_API void WINAPI glEndList( void );
extern OPENGL_API void WINAPI glCallList( GLuint list );
extern OPENGL_API void WINAPI glCallLists( GLsizei n, GLenum type, const GLvoid *lists );
extern OPENGL_API void WINAPI glDeleteLists( GLuint list, GLsizei range );
extern OPENGL_API GLuint WINAPI glGenLists( GLsizei range );
extern OPENGL_API GLboolean WINAPI glIsList( GLuint list );

//==================================================================================
// Display Lists
//==================================================================================

bool g_bCompilingList = false;
bool g_bExecuteList = false;

namespace
{
enum eDisplayListOp : uint8_t
{
    OP_BEGIN = 1,
    OP_END,
    OP_VERTEX3,
    OP_VERTEX4,
    OP_COLOR,
    OP_COLOR2,
    OP_NORMAL,
    OP_TEXCOORD,
    OP_FOG_COORD,

    OP_MATRIX_MODE,
    OP_LOAD_IDENTITY,
    OP_LOAD_MATRIX,
    OP_MULT_MATRIX,
    OP_LOAD_TRANSPOSE_MATRIX,
    OP_MULT_TRANSPOSE_MATRIX,
    OP_FRUSTUM,
    OP_ORTHO,
    OP_PUSH_MATRIX,
    OP_POP_MATRIX,
    OP_ROTATE,
    OP_SCALE,
    OP_TRANSLATE,

    OP_ENABLE,
    OP_DISABLE,
    OP_HINT,
    OP_PUSH_ATTRIB,
    OP_POP_ATTRIB,

    OP_ALPHA_FUNC,
    OP_BLEND_FUNC,
    OP_BLEND_COLOR,
    OP_BLEND_EQUATION,
    OP_FOG_FV,
    OP_CLIP_PLANE,

    OP_LIGHT_MODEL_F,
    OP_LIGHT_MODEL_FV,
    OP_LIGHT_F,
    OP_LIGHT_FV,

    OP_COLOR_MATERIAL,
    OP_MATERIAL_F,
    OP_MATERIAL_FV,

    OP_CLEAR,
    OP_CLEAR_COLOR,
    OP_CLEAR_DEPTH,
    OP_CLEAR_STENCIL,
    OP_COLOR_MASK,
    OP_CULL_FACE,
    OP_FRONT_FACE,
    OP_DEPTH_FUNC,
    OP_DEPTH_MASK,
    OP_DEPTH_RANGE,
    OP_POLYGON_MODE,
    OP_POLYGON_OFFSET,
    OP_SHADE_MODEL,
    OP_POINT_SIZE,
    OP_VIEWPORT,
    OP_SCISSOR,

    OP_STENCIL_FUNC,
    OP_STENCIL_MASK,
    OP_STENCIL_OP,
    OP_ACTIVE_STENCIL_FACE,

    OP_BIND_TEXTURE,
    OP_ACTIVE_TEXTURE,
    OP_SELECT_TEXTURE,
    OP_PRIORITIZE_TEXTURES,
    OP_TEX_ENV_F,
    OP_TEX_ENV_I,
    OP_TEX_ENV_FV,
    OP_TEX_ENV_IV,
    OP_TEX_PARAMETER_F,
    OP_TEX_PARAMETER_I,
    OP_TEX_PARAMETER_FV,
    OP_TEX_PARAMETER_IV,

    OP_TEX_GEN_F,
    OP_TEX_GEN_I,
    OP_TEX_GEN_D,
    OP_TEX_GEN_FV,
    OP_TEX_GEN_IV,
    OP_TEX_GEN_DV,

    OP_PIXEL_TRANSFER_F,
    OP_PIXEL_TRANSFER_I,

    OP_LIST_BASE,
    OP_CALL_LIST,
    OP_CALL_LISTS
};

struct D3DDisplayList
{
    bool allocated = false;
    std::vector<uint8_t> commands;
};

static std::vector<D3DDisplayList> s_lists;
static std::vector<uint8_t> s_stagingBuffer;
static GLuint s_compilingListId = 0;
static GLenum s_compilingListMode = 0;
static GLuint s_listBase = 0;
static int s_callDepth = 0;
static std::vector<GLuint> s_listBaseStack;

// Byte-stream serialization helpers
template<typename T>
inline void WriteVal( std::vector<uint8_t> &buf, const T &val )
{
    const uint8_t *p = reinterpret_cast<const uint8_t*>( &val );
    buf.insert( buf.end(), p, p + sizeof( T ) );
}

inline void WriteBytes( std::vector<uint8_t> &buf, const void *data, size_t size )
{
    const uint8_t *p = reinterpret_cast<const uint8_t*>( data );
    buf.insert( buf.end(), p, p + size );
}

inline void WriteOp( eDisplayListOp op )
{
    s_stagingBuffer.push_back( static_cast<uint8_t>( op ) );
}

template<typename T>
inline T ReadVal( const uint8_t *&ptr )
{
    T val;
    memcpy( &val, ptr, sizeof( T ) );
    ptr += sizeof( T );
    return val;
}

template<typename T>
inline const T* ReadArray( const uint8_t *&ptr, size_t count )
{
    const T *arr = reinterpret_cast<const T*>( ptr );
    ptr += sizeof( T ) * count;
    return arr;
}

static GLint ExtractListOffset( GLenum type, const GLvoid *lists, GLsizei index )
{
    switch ( type )
    {
    case GL_BYTE:
        return static_cast<GLint>( reinterpret_cast<const GLbyte*>( lists )[index] );
    case GL_UNSIGNED_BYTE:
        return static_cast<GLint>( reinterpret_cast<const GLubyte*>( lists )[index] );
    case GL_SHORT:
        return static_cast<GLint>( reinterpret_cast<const GLshort*>( lists )[index] );
    case GL_UNSIGNED_SHORT:
        return static_cast<GLint>( reinterpret_cast<const GLushort*>( lists )[index] );
    case GL_INT:
        return reinterpret_cast<const GLint*>( lists )[index];
    case GL_UNSIGNED_INT:
        return static_cast<GLint>( reinterpret_cast<const GLuint*>( lists )[index] );
    case GL_FLOAT:
        return static_cast<GLint>( reinterpret_cast<const GLfloat*>( lists )[index] );
    case GL_2_BYTES:
    {
        const GLubyte *p = reinterpret_cast<const GLubyte*>( lists ) + index * 2;
        return ( static_cast<GLint>( p[0] ) << 8 ) | static_cast<GLint>( p[1] );
    }
    case GL_3_BYTES:
    {
        const GLubyte *p = reinterpret_cast<const GLubyte*>( lists ) + index * 3;
        return ( static_cast<GLint>( p[0] ) << 16 ) | ( static_cast<GLint>( p[1] ) << 8 ) | static_cast<GLint>( p[2] );
    }
    case GL_4_BYTES:
    {
        const GLubyte *p = reinterpret_cast<const GLubyte*>( lists ) + index * 4;
        return ( static_cast<GLint>( p[0] ) << 24 ) | ( static_cast<GLint>( p[1] ) << 16 ) | ( static_cast<GLint>( p[2] ) << 8 ) | static_cast<GLint>( p[3] );
    }
    default:
        return 0;
    }
}

static bool IsValidCallListsType( GLenum type )
{
    switch ( type )
    {
    case GL_BYTE:
    case GL_UNSIGNED_BYTE:
    case GL_SHORT:
    case GL_UNSIGNED_SHORT:
    case GL_INT:
    case GL_UNSIGNED_INT:
    case GL_FLOAT:
    case GL_2_BYTES:
    case GL_3_BYTES:
    case GL_4_BYTES:
        return true;
    default:
        return false;
    }
}

static void ExecuteSingleList( GLuint list );

static void ReplayCommandStream( const std::vector<uint8_t> &stream )
{
    const uint8_t *ptr = stream.data();
    const uint8_t *end = ptr + stream.size();

    while ( ptr < end )
    {
        eDisplayListOp op = static_cast<eDisplayListOp>( *ptr++ );
        switch ( op )
        {
        case OP_BEGIN:
            glBegin( ReadVal<GLenum>( ptr ) );
            break;
        case OP_END:
            glEnd();
            break;
        case OP_VERTEX3:
        {
            const GLfloat *v = ReadArray<GLfloat>( ptr, 3 );
            assert( D3DGlobal.pIMBuffer != nullptr );
            D3DGlobal.pIMBuffer->AddVertex( v[0], v[1], v[2] );
            break;
        }
        case OP_VERTEX4:
        {
            const GLfloat *v = ReadArray<GLfloat>( ptr, 4 );
            assert( D3DGlobal.pIMBuffer != nullptr );
            D3DGlobal.pIMBuffer->AddVertex( v[0], v[1], v[2], v[3] );
            break;
        }
        case OP_COLOR:
        {
            DWORD color = ReadVal<DWORD>( ptr );
            D3DState.CurrentState.isSet.bits.color = 1;
            D3DState.CurrentState.currentColor = color;
            break;
        }
        case OP_COLOR2:
        {
            DWORD rgb24 = ReadVal<DWORD>( ptr );
            D3DState.CurrentState.isSet.bits.color2 = 1;
            D3DState.CurrentState.currentColor2 = ( D3DState.CurrentState.currentColor2 & 0xFF000000 ) | ( rgb24 & 0x00FFFFFF );
            break;
        }
        case OP_NORMAL:
        {
            const GLfloat *n = ReadArray<GLfloat>( ptr, 3 );
            D3DState.CurrentState.isSet.bits.norm = 1;
            D3DState.CurrentState.currentNormal[0] = n[0];
            D3DState.CurrentState.currentNormal[1] = n[1];
            D3DState.CurrentState.currentNormal[2] = n[2];
            break;
        }
        case OP_TEXCOORD:
        {
            GLenum target = ReadVal<GLenum>( ptr );
            const GLfloat *tc = ReadArray<GLfloat>( ptr, 4 );
            int num = ReadVal<int>( ptr );
            int stage = target;
            if ( stage >= GL_TEXTURE0_ARB ) stage -= GL_TEXTURE0_ARB;
            if ( stage >= 0 && stage < D3DGlobal.maxActiveTMU ) {
                D3DState.CurrentState.isSet.bits.texcoord |= DWORD( num - 1 ) << ( stage * 2 );
                D3DState.CurrentState.currentTexCoord[stage][0] = tc[0];
                D3DState.CurrentState.currentTexCoord[stage][1] = tc[1];
                D3DState.CurrentState.currentTexCoord[stage][2] = tc[2];
                D3DState.CurrentState.currentTexCoord[stage][3] = tc[3];
                if ( D3DState.TransformState.texcoordFixEnabled ) {
                    D3DState.CurrentState.currentTexCoord[stage][0] += D3DState.TransformState.texcoordFix[0];
                    D3DState.CurrentState.currentTexCoord[stage][1] += D3DState.TransformState.texcoordFix[1];
                }
            }
            break;
        }
        case OP_FOG_COORD:
            glFogCoordf( ReadVal<GLfloat>( ptr ) );
            break;

        case OP_MATRIX_MODE:
            glMatrixMode( ReadVal<GLenum>( ptr ) );
            break;
        case OP_LOAD_IDENTITY:
            glLoadIdentity();
            break;
        case OP_LOAD_MATRIX:
            glLoadMatrixf( ReadArray<GLfloat>( ptr, 16 ) );
            break;
        case OP_MULT_MATRIX:
            glMultMatrixf( ReadArray<GLfloat>( ptr, 16 ) );
            break;
        case OP_LOAD_TRANSPOSE_MATRIX:
            glLoadTransposeMatrixf( ReadArray<GLfloat>( ptr, 16 ) );
            break;
        case OP_MULT_TRANSPOSE_MATRIX:
            glMultTransposeMatrixf( ReadArray<GLfloat>( ptr, 16 ) );
            break;
        case OP_FRUSTUM:
        {
            const GLdouble *p = ReadArray<GLdouble>( ptr, 6 );
            glFrustum( p[0], p[1], p[2], p[3], p[4], p[5] );
            break;
        }
        case OP_ORTHO:
        {
            const GLdouble *p = ReadArray<GLdouble>( ptr, 6 );
            glOrtho( p[0], p[1], p[2], p[3], p[4], p[5] );
            break;
        }
        case OP_PUSH_MATRIX:
            glPushMatrix();
            break;
        case OP_POP_MATRIX:
            glPopMatrix();
            break;
        case OP_ROTATE:
        {
            const GLfloat *p = ReadArray<GLfloat>( ptr, 4 );
            glRotatef( p[0], p[1], p[2], p[3] );
            break;
        }
        case OP_SCALE:
        {
            const GLfloat *p = ReadArray<GLfloat>( ptr, 3 );
            glScalef( p[0], p[1], p[2] );
            break;
        }
        case OP_TRANSLATE:
        {
            const GLfloat *p = ReadArray<GLfloat>( ptr, 3 );
            glTranslatef( p[0], p[1], p[2] );
            break;
        }

        case OP_ENABLE:
            glEnable( ReadVal<GLenum>( ptr ) );
            break;
        case OP_DISABLE:
            glDisable( ReadVal<GLenum>( ptr ) );
            break;
        case OP_HINT:
        {
            GLenum target = ReadVal<GLenum>( ptr );
            GLenum mode = ReadVal<GLenum>( ptr );
            glHint( target, mode );
            break;
        }
        case OP_PUSH_ATTRIB:
            glPushAttrib( ReadVal<GLbitfield>( ptr ) );
            break;
        case OP_POP_ATTRIB:
            glPopAttrib();
            break;

        case OP_ALPHA_FUNC:
        {
            GLenum func = ReadVal<GLenum>( ptr );
            GLclampf ref = ReadVal<GLclampf>( ptr );
            glAlphaFunc( func, ref );
            break;
        }
        case OP_BLEND_FUNC:
        {
            GLenum sfactor = ReadVal<GLenum>( ptr );
            GLenum dfactor = ReadVal<GLenum>( ptr );
            glBlendFunc( sfactor, dfactor );
            break;
        }
        case OP_BLEND_COLOR:
        {
            const GLclampf *c = ReadArray<GLclampf>( ptr, 4 );
            glBlendColor( c[0], c[1], c[2], c[3] );
            break;
        }
        case OP_BLEND_EQUATION:
            glBlendEquation( ReadVal<GLenum>( ptr ) );
            break;
        case OP_FOG_FV:
        {
            GLenum pname = ReadVal<GLenum>( ptr );
            const GLfloat *params = ReadArray<GLfloat>( ptr, 4 );
            glFogfv( pname, params );
            break;
        }
        case OP_CLIP_PLANE:
        {
            GLenum plane = ReadVal<GLenum>( ptr );
            const GLdouble *eq = ReadArray<GLdouble>( ptr, 4 );
            glClipPlane( plane, eq );
            break;
        }

        case OP_LIGHT_MODEL_F:
        {
            GLenum pname = ReadVal<GLenum>( ptr );
            GLfloat param = ReadVal<GLfloat>( ptr );
            glLightModelf( pname, param );
            break;
        }
        case OP_LIGHT_MODEL_FV:
        {
            GLenum pname = ReadVal<GLenum>( ptr );
            const GLfloat *params = ReadArray<GLfloat>( ptr, 4 );
            glLightModelfv( pname, params );
            break;
        }
        case OP_LIGHT_F:
        {
            GLenum light = ReadVal<GLenum>( ptr );
            GLenum pname = ReadVal<GLenum>( ptr );
            GLfloat param = ReadVal<GLfloat>( ptr );
            glLightf( light, pname, param );
            break;
        }
        case OP_LIGHT_FV:
        {
            GLenum light = ReadVal<GLenum>( ptr );
            GLenum pname = ReadVal<GLenum>( ptr );
            const GLfloat *params = ReadArray<GLfloat>( ptr, 4 );
            glLightfv( light, pname, params );
            break;
        }

        case OP_COLOR_MATERIAL:
        {
            GLenum face = ReadVal<GLenum>( ptr );
            GLenum mode = ReadVal<GLenum>( ptr );
            glColorMaterial( face, mode );
            break;
        }
        case OP_MATERIAL_F:
        {
            GLenum face = ReadVal<GLenum>( ptr );
            GLenum pname = ReadVal<GLenum>( ptr );
            GLfloat param = ReadVal<GLfloat>( ptr );
            glMaterialf( face, pname, param );
            break;
        }
        case OP_MATERIAL_FV:
        {
            GLenum face = ReadVal<GLenum>( ptr );
            GLenum pname = ReadVal<GLenum>( ptr );
            const GLfloat *params = ReadArray<GLfloat>( ptr, 4 );
            glMaterialfv( face, pname, params );
            break;
        }

        case OP_CLEAR:
            glClear( ReadVal<GLbitfield>( ptr ) );
            break;
        case OP_CLEAR_COLOR:
        {
            const GLclampf *c = ReadArray<GLclampf>( ptr, 4 );
            glClearColor( c[0], c[1], c[2], c[3] );
            break;
        }
        case OP_CLEAR_DEPTH:
            glClearDepth( ReadVal<GLclampd>( ptr ) );
            break;
        case OP_CLEAR_STENCIL:
            glClearStencil( ReadVal<GLint>( ptr ) );
            break;
        case OP_COLOR_MASK:
        {
            const GLboolean *m = ReadArray<GLboolean>( ptr, 4 );
            glColorMask( m[0], m[1], m[2], m[3] );
            break;
        }
        case OP_CULL_FACE:
            glCullFace( ReadVal<GLenum>( ptr ) );
            break;
        case OP_FRONT_FACE:
            glFrontFace( ReadVal<GLenum>( ptr ) );
            break;
        case OP_DEPTH_FUNC:
            glDepthFunc( ReadVal<GLenum>( ptr ) );
            break;
        case OP_DEPTH_MASK:
            glDepthMask( ReadVal<GLboolean>( ptr ) );
            break;
        case OP_DEPTH_RANGE:
        {
            GLclampd zn = ReadVal<GLclampd>( ptr );
            GLclampd zf = ReadVal<GLclampd>( ptr );
            glDepthRange( zn, zf );
            break;
        }
        case OP_POLYGON_MODE:
        {
            GLenum face = ReadVal<GLenum>( ptr );
            GLenum mode = ReadVal<GLenum>( ptr );
            glPolygonMode( face, mode );
            break;
        }
        case OP_POLYGON_OFFSET:
        {
            GLfloat factor = ReadVal<GLfloat>( ptr );
            GLfloat units = ReadVal<GLfloat>( ptr );
            glPolygonOffset( factor, units );
            break;
        }
        case OP_SHADE_MODEL:
            glShadeModel( ReadVal<GLenum>( ptr ) );
            break;
        case OP_POINT_SIZE:
            glPointSize( ReadVal<GLfloat>( ptr ) );
            break;
        case OP_VIEWPORT:
        {
            const GLint *vp = ReadArray<GLint>( ptr, 4 );
            glViewport( vp[0], vp[1], vp[2], vp[3] );
            break;
        }
        case OP_SCISSOR:
        {
            const GLint *sc = ReadArray<GLint>( ptr, 4 );
            glScissor( sc[0], sc[1], sc[2], sc[3] );
            break;
        }

        case OP_STENCIL_FUNC:
        {
            GLenum func = ReadVal<GLenum>( ptr );
            GLint ref = ReadVal<GLint>( ptr );
            GLuint mask = ReadVal<GLuint>( ptr );
            glStencilFunc( func, ref, mask );
            break;
        }
        case OP_STENCIL_MASK:
            glStencilMask( ReadVal<GLuint>( ptr ) );
            break;
        case OP_STENCIL_OP:
        {
            GLenum fail = ReadVal<GLenum>( ptr );
            GLenum zfail = ReadVal<GLenum>( ptr );
            GLenum zpass = ReadVal<GLenum>( ptr );
            glStencilOp( fail, zfail, zpass );
            break;
        }
        case OP_ACTIVE_STENCIL_FACE:
            glActiveStencilFace( ReadVal<GLenum>( ptr ) );
            break;

        case OP_BIND_TEXTURE:
        {
            GLenum target = ReadVal<GLenum>( ptr );
            GLuint texture = ReadVal<GLuint>( ptr );
            glBindTexture( target, texture );
            break;
        }
        case OP_ACTIVE_TEXTURE:
            glActiveTexture( ReadVal<GLenum>( ptr ) );
            break;
        case OP_SELECT_TEXTURE:
            glSelectTexture( ReadVal<GLenum>( ptr ) );
            break;
        case OP_PRIORITIZE_TEXTURES:
        {
            GLsizei n = ReadVal<GLsizei>( ptr );
            const GLuint *textures = ReadArray<GLuint>( ptr, n );
            const GLclampf *priorities = ReadArray<GLclampf>( ptr, n );
            glPrioritizeTextures( n, textures, priorities );
            break;
        }
        case OP_TEX_ENV_F:
        {
            GLenum target = ReadVal<GLenum>( ptr );
            GLenum pname = ReadVal<GLenum>( ptr );
            GLfloat param = ReadVal<GLfloat>( ptr );
            glTexEnvf( target, pname, param );
            break;
        }
        case OP_TEX_ENV_I:
        {
            GLenum target = ReadVal<GLenum>( ptr );
            GLenum pname = ReadVal<GLenum>( ptr );
            GLint param = ReadVal<GLint>( ptr );
            glTexEnvi( target, pname, param );
            break;
        }
        case OP_TEX_ENV_FV:
        {
            GLenum target = ReadVal<GLenum>( ptr );
            GLenum pname = ReadVal<GLenum>( ptr );
            const GLfloat *params = ReadArray<GLfloat>( ptr, 4 );
            glTexEnvfv( target, pname, params );
            break;
        }
        case OP_TEX_ENV_IV:
        {
            GLenum target = ReadVal<GLenum>( ptr );
            GLenum pname = ReadVal<GLenum>( ptr );
            const GLint *params = ReadArray<GLint>( ptr, 4 );
            glTexEnviv( target, pname, params );
            break;
        }
        case OP_TEX_PARAMETER_F:
        {
            GLenum target = ReadVal<GLenum>( ptr );
            GLenum pname = ReadVal<GLenum>( ptr );
            GLfloat param = ReadVal<GLfloat>( ptr );
            glTexParameterf( target, pname, param );
            break;
        }
        case OP_TEX_PARAMETER_I:
        {
            GLenum target = ReadVal<GLenum>( ptr );
            GLenum pname = ReadVal<GLenum>( ptr );
            GLint param = ReadVal<GLint>( ptr );
            glTexParameteri( target, pname, param );
            break;
        }
        case OP_TEX_PARAMETER_FV:
        {
            GLenum target = ReadVal<GLenum>( ptr );
            GLenum pname = ReadVal<GLenum>( ptr );
            const GLfloat *params = ReadArray<GLfloat>( ptr, 4 );
            glTexParameterfv( target, pname, params );
            break;
        }
        case OP_TEX_PARAMETER_IV:
        {
            GLenum target = ReadVal<GLenum>( ptr );
            GLenum pname = ReadVal<GLenum>( ptr );
            const GLint *params = ReadArray<GLint>( ptr, 4 );
            glTexParameteriv( target, pname, params );
            break;
        }

        case OP_TEX_GEN_F:
        {
            GLenum coord = ReadVal<GLenum>( ptr );
            GLenum pname = ReadVal<GLenum>( ptr );
            GLfloat param = ReadVal<GLfloat>( ptr );
            glTexGenf( coord, pname, param );
            break;
        }
        case OP_TEX_GEN_I:
        {
            GLenum coord = ReadVal<GLenum>( ptr );
            GLenum pname = ReadVal<GLenum>( ptr );
            GLint param = ReadVal<GLint>( ptr );
            glTexGeni( coord, pname, param );
            break;
        }
        case OP_TEX_GEN_D:
        {
            GLenum coord = ReadVal<GLenum>( ptr );
            GLenum pname = ReadVal<GLenum>( ptr );
            GLdouble param = ReadVal<GLdouble>( ptr );
            glTexGend( coord, pname, param );
            break;
        }
        case OP_TEX_GEN_FV:
        {
            GLenum coord = ReadVal<GLenum>( ptr );
            GLenum pname = ReadVal<GLenum>( ptr );
            const GLfloat *params = ReadArray<GLfloat>( ptr, 4 );
            glTexGenfv( coord, pname, params );
            break;
        }
        case OP_TEX_GEN_IV:
        {
            GLenum coord = ReadVal<GLenum>( ptr );
            GLenum pname = ReadVal<GLenum>( ptr );
            const GLint *params = ReadArray<GLint>( ptr, 4 );
            glTexGeniv( coord, pname, params );
            break;
        }
        case OP_TEX_GEN_DV:
        {
            GLenum coord = ReadVal<GLenum>( ptr );
            GLenum pname = ReadVal<GLenum>( ptr );
            const GLdouble *params = ReadArray<GLdouble>( ptr, 4 );
            glTexGendv( coord, pname, params );
            break;
        }

        case OP_PIXEL_TRANSFER_F:
        {
            GLenum pname = ReadVal<GLenum>( ptr );
            GLfloat param = ReadVal<GLfloat>( ptr );
            glPixelTransferf( pname, param );
            break;
        }
        case OP_PIXEL_TRANSFER_I:
        {
            GLenum pname = ReadVal<GLenum>( ptr );
            GLint param = ReadVal<GLint>( ptr );
            glPixelTransferi( pname, param );
            break;
        }

        case OP_LIST_BASE:
            s_listBase = ReadVal<GLuint>( ptr );
            break;
        case OP_CALL_LIST:
            ExecuteSingleList( ReadVal<GLuint>( ptr ) );
            break;
        case OP_CALL_LISTS:
        {
            GLsizei n = ReadVal<GLsizei>( ptr );
            const GLint *offsets = ReadArray<GLint>( ptr, n );
            for ( GLsizei i = 0; i < n; ++i ) {
                GLint id = static_cast<GLint>( s_listBase ) + offsets[i];
                if ( id > 0 )
                    ExecuteSingleList( static_cast<GLuint>( id ) );
            }
            break;
        }
        default:
            logPrintf( "ERROR: ReplayCommandStream: unknown opcode %u\n", static_cast<unsigned>( op ) );
            return;
        }
    }
}

static void ExecuteSingleList( GLuint list )
{
    if ( list == 0 || list >= s_lists.size() )
        return;
    if ( !s_lists[list].allocated || s_lists[list].commands.empty() )
        return;
    if ( s_callDepth >= IMPL_MAX_LIST_NESTING )
        return;

    // Suspend recording while replaying a called list during GL_COMPILE_AND_EXECUTE
    bool prevCompiling = g_bCompilingList;
    g_bCompilingList = false;
    ++s_callDepth;

    // Keep a local copy in case the list redefines or deletes lists
    const std::vector<uint8_t> &cmds = s_lists[list].commands;
    ReplayCommandStream( cmds );

    --s_callDepth;
    g_bCompilingList = prevCompiling;
}
}

//==================================================================================
// Lifecycle & State Queries
//==================================================================================

void D3DLists_Init()
{
    s_lists.clear();
    s_lists.resize( 1 ); // index 0 is unused
    s_stagingBuffer.clear();
    s_listBaseStack.clear();
    g_bCompilingList = false;
    g_bExecuteList = false;
    s_compilingListId = 0;
    s_compilingListMode = 0;
    s_listBase = 0;
    s_callDepth = 0;
}

void D3DLists_Cleanup()
{
    size_t totalBytes = 0;
    size_t activeLists = 0;
    for ( size_t i = 1; i < s_lists.size(); ++i ) {
        if ( s_lists[i].allocated ) {
            ++activeLists;
            totalBytes += s_lists[i].commands.capacity();
        }
    }
    if ( activeLists > 0 ) {
        logPrintf( "D3DLists: %.2f kb in %u display lists\n", totalBytes / 1024.0f, static_cast<unsigned>( activeLists ) );
    }
    s_lists.clear();
    s_stagingBuffer.clear();
    s_listBaseStack.clear();
    g_bCompilingList = false;
    g_bExecuteList = false;
    s_compilingListId = 0;
    s_compilingListMode = 0;
    s_listBase = 0;
    s_callDepth = 0;
}

GLuint D3DLists_GetListBase()  { return s_listBase; }
GLuint D3DLists_GetListIndex() { return s_compilingListId; }
GLenum D3DLists_GetListMode()  { return s_compilingListMode; }

void D3DLists_PushAttrib()
{
    s_listBaseStack.push_back( s_listBase );
}

void D3DLists_PopAttrib()
{
    if ( !s_listBaseStack.empty() ) {
        s_listBase = s_listBaseStack.back();
        s_listBaseStack.pop_back();
    }
}

//==================================================================================
// OpenGL Display Lists API Entry Points
//==================================================================================

OPENGL_API void WINAPI glListBase( GLuint base )
{
    if ( g_bCompilingList ) {
        WriteOp( OP_LIST_BASE );
        WriteVal( s_stagingBuffer, base );
        if ( !g_bExecuteList )
            return;
    }
    s_listBase = base;
}

OPENGL_API GLuint WINAPI glGenLists( GLsizei range )
{
    if ( range < 0 ) {
        D3DGlobal.lastError = E_INVALIDARG;
        return 0;
    }
    if ( range == 0 )
        return 0;

    if ( s_lists.empty() )
        s_lists.resize( 1 );

    // Search for a contiguous block of 'range' unallocated list IDs
    const size_t total = s_lists.size();
    for ( size_t i = 1; i + static_cast<size_t>( range ) <= total; ) {
        size_t j = 0;
        for ( ; j < static_cast<size_t>( range ); ++j ) {
            if ( s_lists[i + j].allocated )
                break;
        }
        if ( j < static_cast<size_t>( range ) ) {
            i += j + 1;
            continue;
        }
        for ( size_t k = 0; k < static_cast<size_t>( range ); ++k ) {
            s_lists[i + k].allocated = true;
            s_lists[i + k].commands.clear();
        }
        return static_cast<GLuint>( i );
    }

    // Extend list array at the end
    GLuint baseId = static_cast<GLuint>( s_lists.size() );
    s_lists.resize( baseId + range );
    for ( GLsizei k = 0; k < range; ++k ) {
        s_lists[baseId + k].allocated = true;
    }
    return baseId;
}

OPENGL_API void WINAPI glDeleteLists( GLuint list, GLsizei range )
{
    if ( range < 0 ) {
        D3DGlobal.lastError = E_INVALIDARG;
        return;
    }
    if ( list == 0 || range == 0 )
        return;

    for ( GLsizei i = 0; i < range; ++i ) {
        GLuint id = list + i;
        if ( id > 0 && id < s_lists.size() ) {
            s_lists[id].allocated = false;
            s_lists[id].commands.clear();
            s_lists[id].commands.shrink_to_fit();
        }
    }
}

OPENGL_API GLboolean WINAPI glIsList( GLuint list )
{
    if ( list == 0 || list >= s_lists.size() )
        return GL_FALSE;
    return s_lists[list].allocated ? GL_TRUE : GL_FALSE;
}

OPENGL_API void WINAPI glNewList( GLuint list, GLenum mode )
{
    if ( list == 0 ) {
        D3DGlobal.lastError = E_INVALIDARG;
        return;
    }
    if ( mode != GL_COMPILE && mode != GL_COMPILE_AND_EXECUTE ) {
        D3DGlobal.lastError = E_INVALID_ENUM;
        return;
    }
    if ( g_bCompilingList ) {
        D3DGlobal.lastError = E_INVALID_OPERATION;
        return;
    }

    s_compilingListId = list;
    s_compilingListMode = mode;
    s_stagingBuffer.clear();
    g_bCompilingList = true;
    g_bExecuteList = ( mode == GL_COMPILE_AND_EXECUTE );
}

OPENGL_API void WINAPI glEndList()
{
    if ( !g_bCompilingList ) {
        D3DGlobal.lastError = E_INVALID_OPERATION;
        return;
    }

    if ( s_compilingListId >= s_lists.size() ) {
        s_lists.resize( s_compilingListId + 1 );
    }

    s_lists[s_compilingListId].allocated = true;
    s_lists[s_compilingListId].commands = std::move( s_stagingBuffer );
    s_stagingBuffer.clear();

    g_bCompilingList = false;
    g_bExecuteList = false;
    s_compilingListId = 0;
    s_compilingListMode = 0;
}

OPENGL_API void WINAPI glCallList( GLuint list )
{
    if ( list == 0 ) {
        D3DGlobal.lastError = E_INVALIDARG;
        return;
    }

    if ( g_bCompilingList ) {
        WriteOp( OP_CALL_LIST );
        WriteVal( s_stagingBuffer, list );
        if ( !g_bExecuteList )
            return;
    }

    ExecuteSingleList( list );
}

OPENGL_API void WINAPI glCallLists( GLsizei n, GLenum type, const GLvoid *lists )
{
    if ( n < 0 ) {
        D3DGlobal.lastError = E_INVALIDARG;
        return;
    }
    if ( !IsValidCallListsType( type ) ) {
        D3DGlobal.lastError = E_INVALID_ENUM;
        return;
    }
    if ( n == 0 || !lists )
        return;

    if ( g_bCompilingList ) {
        WriteOp( OP_CALL_LISTS );
        WriteVal( s_stagingBuffer, n );
        for ( GLsizei i = 0; i < n; ++i ) {
            GLint offset = ExtractListOffset( type, lists, i );
            WriteVal( s_stagingBuffer, offset );
        }
        if ( !g_bExecuteList )
            return;
    }

    for ( GLsizei i = 0; i < n; ++i ) {
        GLint offset = ExtractListOffset( type, lists, i );
        GLint id = static_cast<GLint>( s_listBase ) + offset;
        if ( id > 0 )
            ExecuteSingleList( static_cast<GLuint>( id ) );
    }
}

//==================================================================================
// Recording Helpers
//==================================================================================

void D3DLists_RecordBegin( GLenum mode )
{
    WriteOp( OP_BEGIN );
    WriteVal( s_stagingBuffer, mode );
}

void D3DLists_RecordEnd()
{
    WriteOp( OP_END );
}

void D3DLists_RecordVertex3( GLfloat x, GLfloat y, GLfloat z )
{
    WriteOp( OP_VERTEX3 );
    WriteVal( s_stagingBuffer, x );
    WriteVal( s_stagingBuffer, y );
    WriteVal( s_stagingBuffer, z );
}

void D3DLists_RecordVertex4( GLfloat x, GLfloat y, GLfloat z, GLfloat w )
{
    WriteOp( OP_VERTEX4 );
    WriteVal( s_stagingBuffer, x );
    WriteVal( s_stagingBuffer, y );
    WriteVal( s_stagingBuffer, z );
    WriteVal( s_stagingBuffer, w );
}

void D3DLists_RecordColor( DWORD color )
{
    WriteOp( OP_COLOR );
    WriteVal( s_stagingBuffer, color );
}

void D3DLists_RecordColor2( DWORD color2Rgb )
{
    WriteOp( OP_COLOR2 );
    WriteVal( s_stagingBuffer, color2Rgb );
}

void D3DLists_RecordNormal( GLfloat nx, GLfloat ny, GLfloat nz )
{
    WriteOp( OP_NORMAL );
    WriteVal( s_stagingBuffer, nx );
    WriteVal( s_stagingBuffer, ny );
    WriteVal( s_stagingBuffer, nz );
}

void D3DLists_RecordTexCoord( GLenum target, GLfloat s, GLfloat t, GLfloat r, GLfloat q, int num )
{
    WriteOp( OP_TEXCOORD );
    WriteVal( s_stagingBuffer, target );
    WriteVal( s_stagingBuffer, s );
    WriteVal( s_stagingBuffer, t );
    WriteVal( s_stagingBuffer, r );
    WriteVal( s_stagingBuffer, q );
    WriteVal( s_stagingBuffer, num );
}

void D3DLists_RecordFogCoord( GLfloat coord )
{
    WriteOp( OP_FOG_COORD );
    WriteVal( s_stagingBuffer, coord );
}

void D3DLists_RecordMatrixMode( GLenum mode )
{
    WriteOp( OP_MATRIX_MODE );
    WriteVal( s_stagingBuffer, mode );
}

void D3DLists_RecordLoadIdentity()
{
    WriteOp( OP_LOAD_IDENTITY );
}

void D3DLists_RecordLoadMatrix( const GLfloat *m )
{
    WriteOp( OP_LOAD_MATRIX );
    WriteBytes( s_stagingBuffer, m, sizeof( GLfloat ) * 16 );
}

void D3DLists_RecordMultMatrix( const GLfloat *m )
{
    WriteOp( OP_MULT_MATRIX );
    WriteBytes( s_stagingBuffer, m, sizeof( GLfloat ) * 16 );
}

void D3DLists_RecordLoadTransposeMatrix( const GLfloat *m )
{
    WriteOp( OP_LOAD_TRANSPOSE_MATRIX );
    WriteBytes( s_stagingBuffer, m, sizeof( GLfloat ) * 16 );
}

void D3DLists_RecordMultTransposeMatrix( const GLfloat *m )
{
    WriteOp( OP_MULT_TRANSPOSE_MATRIX );
    WriteBytes( s_stagingBuffer, m, sizeof( GLfloat ) * 16 );
}

void D3DLists_RecordFrustum( GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble zNear, GLdouble zFar )
{
    WriteOp( OP_FRUSTUM );
    WriteVal( s_stagingBuffer, left );
    WriteVal( s_stagingBuffer, right );
    WriteVal( s_stagingBuffer, bottom );
    WriteVal( s_stagingBuffer, top );
    WriteVal( s_stagingBuffer, zNear );
    WriteVal( s_stagingBuffer, zFar );
}

void D3DLists_RecordOrtho( GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble zNear, GLdouble zFar )
{
    WriteOp( OP_ORTHO );
    WriteVal( s_stagingBuffer, left );
    WriteVal( s_stagingBuffer, right );
    WriteVal( s_stagingBuffer, bottom );
    WriteVal( s_stagingBuffer, top );
    WriteVal( s_stagingBuffer, zNear );
    WriteVal( s_stagingBuffer, zFar );
}

void D3DLists_RecordPushMatrix()
{
    WriteOp( OP_PUSH_MATRIX );
}

void D3DLists_RecordPopMatrix()
{
    WriteOp( OP_POP_MATRIX );
}

void D3DLists_RecordRotate( GLfloat angle, GLfloat x, GLfloat y, GLfloat z )
{
    WriteOp( OP_ROTATE );
    WriteVal( s_stagingBuffer, angle );
    WriteVal( s_stagingBuffer, x );
    WriteVal( s_stagingBuffer, y );
    WriteVal( s_stagingBuffer, z );
}

void D3DLists_RecordScale( GLfloat x, GLfloat y, GLfloat z )
{
    WriteOp( OP_SCALE );
    WriteVal( s_stagingBuffer, x );
    WriteVal( s_stagingBuffer, y );
    WriteVal( s_stagingBuffer, z );
}

void D3DLists_RecordTranslate( GLfloat x, GLfloat y, GLfloat z )
{
    WriteOp( OP_TRANSLATE );
    WriteVal( s_stagingBuffer, x );
    WriteVal( s_stagingBuffer, y );
    WriteVal( s_stagingBuffer, z );
}

void D3DLists_RecordEnable( GLenum cap )
{
    WriteOp( OP_ENABLE );
    WriteVal( s_stagingBuffer, cap );
}

void D3DLists_RecordDisable( GLenum cap )
{
    WriteOp( OP_DISABLE );
    WriteVal( s_stagingBuffer, cap );
}

void D3DLists_RecordHint( GLenum target, GLenum mode )
{
    WriteOp( OP_HINT );
    WriteVal( s_stagingBuffer, target );
    WriteVal( s_stagingBuffer, mode );
}

void D3DLists_RecordPushAttrib( GLbitfield mask )
{
    WriteOp( OP_PUSH_ATTRIB );
    WriteVal( s_stagingBuffer, mask );
}

void D3DLists_RecordPopAttrib()
{
    WriteOp( OP_POP_ATTRIB );
}

void D3DLists_RecordAlphaFunc( GLenum func, GLclampf ref )
{
    WriteOp( OP_ALPHA_FUNC );
    WriteVal( s_stagingBuffer, func );
    WriteVal( s_stagingBuffer, ref );
}

void D3DLists_RecordBlendFunc( GLenum sfactor, GLenum dfactor )
{
    WriteOp( OP_BLEND_FUNC );
    WriteVal( s_stagingBuffer, sfactor );
    WriteVal( s_stagingBuffer, dfactor );
}

void D3DLists_RecordBlendColor( GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha )
{
    WriteOp( OP_BLEND_COLOR );
    WriteVal( s_stagingBuffer, red );
    WriteVal( s_stagingBuffer, green );
    WriteVal( s_stagingBuffer, blue );
    WriteVal( s_stagingBuffer, alpha );
}

void D3DLists_RecordBlendEquation( GLenum mode )
{
    WriteOp( OP_BLEND_EQUATION );
    WriteVal( s_stagingBuffer, mode );
}

void D3DLists_RecordFogfv( GLenum pname, const GLfloat *params )
{
    GLfloat safeParams[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
    int count = ( pname == GL_FOG_COLOR ) ? 4 : 1;
    for ( int i = 0; i < count; ++i )
        safeParams[i] = params[i];

    WriteOp( OP_FOG_FV );
    WriteVal( s_stagingBuffer, pname );
    WriteBytes( s_stagingBuffer, safeParams, sizeof( safeParams ) );
}

void D3DLists_RecordClipPlane( GLenum plane, const GLdouble *equation )
{
    WriteOp( OP_CLIP_PLANE );
    WriteVal( s_stagingBuffer, plane );
    WriteBytes( s_stagingBuffer, equation, sizeof( GLdouble ) * 4 );
}

void D3DLists_RecordLightModelf( GLenum pname, GLfloat param )
{
    WriteOp( OP_LIGHT_MODEL_F );
    WriteVal( s_stagingBuffer, pname );
    WriteVal( s_stagingBuffer, param );
}

void D3DLists_RecordLightModelfv( GLenum pname, const GLfloat *params )
{
    GLfloat safeParams[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
    int count = ( pname == GL_LIGHT_MODEL_AMBIENT ) ? 4 : 1;
    for ( int i = 0; i < count; ++i )
        safeParams[i] = params[i];

    WriteOp( OP_LIGHT_MODEL_FV );
    WriteVal( s_stagingBuffer, pname );
    WriteBytes( s_stagingBuffer, safeParams, sizeof( safeParams ) );
}

void D3DLists_RecordLightf( GLenum light, GLenum pname, GLfloat param )
{
    WriteOp( OP_LIGHT_F );
    WriteVal( s_stagingBuffer, light );
    WriteVal( s_stagingBuffer, pname );
    WriteVal( s_stagingBuffer, param );
}

void D3DLists_RecordLightfv( GLenum light, GLenum pname, const GLfloat *params )
{
    GLfloat safeParams[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
    int count = 1;
    if ( pname == GL_AMBIENT || pname == GL_DIFFUSE || pname == GL_SPECULAR || pname == GL_POSITION )
        count = 4;
    else if ( pname == GL_SPOT_DIRECTION )
        count = 3;
    for ( int i = 0; i < count; ++i )
        safeParams[i] = params[i];

    WriteOp( OP_LIGHT_FV );
    WriteVal( s_stagingBuffer, light );
    WriteVal( s_stagingBuffer, pname );
    WriteBytes( s_stagingBuffer, safeParams, sizeof( safeParams ) );
}

void D3DLists_RecordColorMaterial( GLenum face, GLenum mode )
{
    WriteOp( OP_COLOR_MATERIAL );
    WriteVal( s_stagingBuffer, face );
    WriteVal( s_stagingBuffer, mode );
}

void D3DLists_RecordMaterialf( GLenum face, GLenum pname, GLfloat param )
{
    WriteOp( OP_MATERIAL_F );
    WriteVal( s_stagingBuffer, face );
    WriteVal( s_stagingBuffer, pname );
    WriteVal( s_stagingBuffer, param );
}

void D3DLists_RecordMaterialfv( GLenum face, GLenum pname, const GLfloat *params )
{
    GLfloat safeParams[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
    int count = ( pname == GL_SHININESS ) ? 1 : 4;
    for ( int i = 0; i < count; ++i )
        safeParams[i] = params[i];

    WriteOp( OP_MATERIAL_FV );
    WriteVal( s_stagingBuffer, face );
    WriteVal( s_stagingBuffer, pname );
    WriteBytes( s_stagingBuffer, safeParams, sizeof( safeParams ) );
}

void D3DLists_RecordClear( GLbitfield mask )
{
    WriteOp( OP_CLEAR );
    WriteVal( s_stagingBuffer, mask );
}

void D3DLists_RecordClearColor( GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha )
{
    WriteOp( OP_CLEAR_COLOR );
    WriteVal( s_stagingBuffer, red );
    WriteVal( s_stagingBuffer, green );
    WriteVal( s_stagingBuffer, blue );
    WriteVal( s_stagingBuffer, alpha );
}

void D3DLists_RecordClearDepth( GLclampd depth )
{
    WriteOp( OP_CLEAR_DEPTH );
    WriteVal( s_stagingBuffer, depth );
}

void D3DLists_RecordClearStencil( GLint s )
{
    WriteOp( OP_CLEAR_STENCIL );
    WriteVal( s_stagingBuffer, s );
}

void D3DLists_RecordColorMask( GLboolean red, GLboolean green, GLboolean blue, GLboolean alpha )
{
    WriteOp( OP_COLOR_MASK );
    WriteVal( s_stagingBuffer, red );
    WriteVal( s_stagingBuffer, green );
    WriteVal( s_stagingBuffer, blue );
    WriteVal( s_stagingBuffer, alpha );
}

void D3DLists_RecordCullFace( GLenum mode )
{
    WriteOp( OP_CULL_FACE );
    WriteVal( s_stagingBuffer, mode );
}

void D3DLists_RecordFrontFace( GLenum mode )
{
    WriteOp( OP_FRONT_FACE );
    WriteVal( s_stagingBuffer, mode );
}

void D3DLists_RecordDepthFunc( GLenum func )
{
    WriteOp( OP_DEPTH_FUNC );
    WriteVal( s_stagingBuffer, func );
}

void D3DLists_RecordDepthMask( GLboolean flag )
{
    WriteOp( OP_DEPTH_MASK );
    WriteVal( s_stagingBuffer, flag );
}

void D3DLists_RecordDepthRange( GLclampd zNear, GLclampd zFar )
{
    WriteOp( OP_DEPTH_RANGE );
    WriteVal( s_stagingBuffer, zNear );
    WriteVal( s_stagingBuffer, zFar );
}

void D3DLists_RecordPolygonMode( GLenum face, GLenum mode )
{
    WriteOp( OP_POLYGON_MODE );
    WriteVal( s_stagingBuffer, face );
    WriteVal( s_stagingBuffer, mode );
}

void D3DLists_RecordPolygonOffset( GLfloat factor, GLfloat units )
{
    WriteOp( OP_POLYGON_OFFSET );
    WriteVal( s_stagingBuffer, factor );
    WriteVal( s_stagingBuffer, units );
}

void D3DLists_RecordShadeModel( GLenum mode )
{
    WriteOp( OP_SHADE_MODEL );
    WriteVal( s_stagingBuffer, mode );
}

void D3DLists_RecordPointSize( GLfloat size )
{
    WriteOp( OP_POINT_SIZE );
    WriteVal( s_stagingBuffer, size );
}

void D3DLists_RecordViewport( GLint x, GLint y, GLsizei width, GLsizei height )
{
    WriteOp( OP_VIEWPORT );
    WriteVal( s_stagingBuffer, x );
    WriteVal( s_stagingBuffer, y );
    WriteVal( s_stagingBuffer, width );
    WriteVal( s_stagingBuffer, height );
}

void D3DLists_RecordScissor( GLint x, GLint y, GLsizei width, GLsizei height )
{
    WriteOp( OP_SCISSOR );
    WriteVal( s_stagingBuffer, x );
    WriteVal( s_stagingBuffer, y );
    WriteVal( s_stagingBuffer, width );
    WriteVal( s_stagingBuffer, height );
}

void D3DLists_RecordStencilFunc( GLenum func, GLint ref, GLuint mask )
{
    WriteOp( OP_STENCIL_FUNC );
    WriteVal( s_stagingBuffer, func );
    WriteVal( s_stagingBuffer, ref );
    WriteVal( s_stagingBuffer, mask );
}

void D3DLists_RecordStencilMask( GLuint mask )
{
    WriteOp( OP_STENCIL_MASK );
    WriteVal( s_stagingBuffer, mask );
}

void D3DLists_RecordStencilOp( GLenum fail, GLenum zfail, GLenum zpass )
{
    WriteOp( OP_STENCIL_OP );
    WriteVal( s_stagingBuffer, fail );
    WriteVal( s_stagingBuffer, zfail );
    WriteVal( s_stagingBuffer, zpass );
}

void D3DLists_RecordActiveStencilFace( GLenum face )
{
    WriteOp( OP_ACTIVE_STENCIL_FACE );
    WriteVal( s_stagingBuffer, face );
}

void D3DLists_RecordBindTexture( GLenum target, GLuint texture )
{
    WriteOp( OP_BIND_TEXTURE );
    WriteVal( s_stagingBuffer, target );
    WriteVal( s_stagingBuffer, texture );
}

void D3DLists_RecordActiveTexture( GLenum texture )
{
    WriteOp( OP_ACTIVE_TEXTURE );
    WriteVal( s_stagingBuffer, texture );
}

void D3DLists_RecordSelectTexture( GLenum texture )
{
    WriteOp( OP_SELECT_TEXTURE );
    WriteVal( s_stagingBuffer, texture );
}

void D3DLists_RecordPrioritizeTextures( GLsizei n, const GLuint *textures, const GLclampf *priorities )
{
    if ( n <= 0 || !textures || !priorities )
        return;
    WriteOp( OP_PRIORITIZE_TEXTURES );
    WriteVal( s_stagingBuffer, n );
    WriteBytes( s_stagingBuffer, textures, sizeof( GLuint ) * n );
    WriteBytes( s_stagingBuffer, priorities, sizeof( GLclampf ) * n );
}

void D3DLists_RecordTexEnvf( GLenum target, GLenum pname, GLfloat param )
{
    WriteOp( OP_TEX_ENV_F );
    WriteVal( s_stagingBuffer, target );
    WriteVal( s_stagingBuffer, pname );
    WriteVal( s_stagingBuffer, param );
}

void D3DLists_RecordTexEnvi( GLenum target, GLenum pname, GLint param )
{
    WriteOp( OP_TEX_ENV_I );
    WriteVal( s_stagingBuffer, target );
    WriteVal( s_stagingBuffer, pname );
    WriteVal( s_stagingBuffer, param );
}

void D3DLists_RecordTexEnvfv( GLenum target, GLenum pname, const GLfloat *params )
{
    GLfloat safeParams[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
    int count = ( pname == GL_TEXTURE_ENV_COLOR ) ? 4 : 1;
    for ( int i = 0; i < count; ++i )
        safeParams[i] = params[i];

    WriteOp( OP_TEX_ENV_FV );
    WriteVal( s_stagingBuffer, target );
    WriteVal( s_stagingBuffer, pname );
    WriteBytes( s_stagingBuffer, safeParams, sizeof( safeParams ) );
}

void D3DLists_RecordTexEnviv( GLenum target, GLenum pname, const GLint *params )
{
    GLint safeParams[4] = { 0, 0, 0, 0 };
    int count = ( pname == GL_TEXTURE_ENV_COLOR ) ? 4 : 1;
    for ( int i = 0; i < count; ++i )
        safeParams[i] = params[i];

    WriteOp( OP_TEX_ENV_IV );
    WriteVal( s_stagingBuffer, target );
    WriteVal( s_stagingBuffer, pname );
    WriteBytes( s_stagingBuffer, safeParams, sizeof( safeParams ) );
}

void D3DLists_RecordTexParameterf( GLenum target, GLenum pname, GLfloat param )
{
    WriteOp( OP_TEX_PARAMETER_F );
    WriteVal( s_stagingBuffer, target );
    WriteVal( s_stagingBuffer, pname );
    WriteVal( s_stagingBuffer, param );
}

void D3DLists_RecordTexParameteri( GLenum target, GLenum pname, GLint param )
{
    WriteOp( OP_TEX_PARAMETER_I );
    WriteVal( s_stagingBuffer, target );
    WriteVal( s_stagingBuffer, pname );
    WriteVal( s_stagingBuffer, param );
}

void D3DLists_RecordTexParameterfv( GLenum target, GLenum pname, const GLfloat *params )
{
    GLfloat safeParams[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
    int count = ( pname == GL_TEXTURE_BORDER_COLOR ) ? 4 : 1;
    for ( int i = 0; i < count; ++i )
        safeParams[i] = params[i];

    WriteOp( OP_TEX_PARAMETER_FV );
    WriteVal( s_stagingBuffer, target );
    WriteVal( s_stagingBuffer, pname );
    WriteBytes( s_stagingBuffer, safeParams, sizeof( safeParams ) );
}

void D3DLists_RecordTexParameteriv( GLenum target, GLenum pname, const GLint *params )
{
    GLint safeParams[4] = { 0, 0, 0, 0 };
    int count = ( pname == GL_TEXTURE_BORDER_COLOR ) ? 4 : 1;
    for ( int i = 0; i < count; ++i )
        safeParams[i] = params[i];

    WriteOp( OP_TEX_PARAMETER_IV );
    WriteVal( s_stagingBuffer, target );
    WriteVal( s_stagingBuffer, pname );
    WriteBytes( s_stagingBuffer, safeParams, sizeof( safeParams ) );
}

void D3DLists_RecordTexGenf( GLenum coord, GLenum pname, GLfloat param )
{
    WriteOp( OP_TEX_GEN_F );
    WriteVal( s_stagingBuffer, coord );
    WriteVal( s_stagingBuffer, pname );
    WriteVal( s_stagingBuffer, param );
}

void D3DLists_RecordTexGeni( GLenum coord, GLenum pname, GLint param )
{
    WriteOp( OP_TEX_GEN_I );
    WriteVal( s_stagingBuffer, coord );
    WriteVal( s_stagingBuffer, pname );
    WriteVal( s_stagingBuffer, param );
}

void D3DLists_RecordTexGend( GLenum coord, GLenum pname, GLdouble param )
{
    WriteOp( OP_TEX_GEN_D );
    WriteVal( s_stagingBuffer, coord );
    WriteVal( s_stagingBuffer, pname );
    WriteVal( s_stagingBuffer, param );
}

void D3DLists_RecordTexGenfv( GLenum coord, GLenum pname, const GLfloat *params )
{
    GLfloat safeParams[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
    int count = ( pname == GL_TEXTURE_GEN_MODE ) ? 1 : 4;
    for ( int i = 0; i < count; ++i )
        safeParams[i] = params[i];

    WriteOp( OP_TEX_GEN_FV );
    WriteVal( s_stagingBuffer, coord );
    WriteVal( s_stagingBuffer, pname );
    WriteBytes( s_stagingBuffer, safeParams, sizeof( safeParams ) );
}

void D3DLists_RecordTexGeniv( GLenum coord, GLenum pname, const GLint *params )
{
    GLint safeParams[4] = { 0, 0, 0, 0 };
    int count = ( pname == GL_TEXTURE_GEN_MODE ) ? 1 : 4;
    for ( int i = 0; i < count; ++i )
        safeParams[i] = params[i];

    WriteOp( OP_TEX_GEN_IV );
    WriteVal( s_stagingBuffer, coord );
    WriteVal( s_stagingBuffer, pname );
    WriteBytes( s_stagingBuffer, safeParams, sizeof( safeParams ) );
}

void D3DLists_RecordTexGendv( GLenum coord, GLenum pname, const GLdouble *params )
{
    GLdouble safeParams[4] = { 0.0, 0.0, 0.0, 0.0 };
    int count = ( pname == GL_TEXTURE_GEN_MODE ) ? 1 : 4;
    for ( int i = 0; i < count; ++i )
        safeParams[i] = params[i];

    WriteOp( OP_TEX_GEN_DV );
    WriteVal( s_stagingBuffer, coord );
    WriteVal( s_stagingBuffer, pname );
    WriteBytes( s_stagingBuffer, safeParams, sizeof( safeParams ) );
}

void D3DLists_RecordPixelTransferf( GLenum pname, GLfloat param )
{
    WriteOp( OP_PIXEL_TRANSFER_F );
    WriteVal( s_stagingBuffer, pname );
    WriteVal( s_stagingBuffer, param );
}

void D3DLists_RecordPixelTransferi( GLenum pname, GLint param )
{
    WriteOp( OP_PIXEL_TRANSFER_I );
    WriteVal( s_stagingBuffer, pname );
    WriteVal( s_stagingBuffer, param );
}
