/*------------------------------------------------------------------------------
Copyright (c) 2009 The GRRLIB Team

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
------------------------------------------------------------------------------*/

#include <grrlib.h>
#include <malloc.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


/**
 * Shortcut to access triangles values.
 */
#define T(x) model->triangles[(x)]

/**
 * Elements of a vertex
 */
enum { X, Y, Z, W };

/**
 * Find a group in the model.
 * @param model Structure that defines the model in wich the group will be searched.
 * @param name Yhe name of the group to find.
 */
static GRRLIB_Group* GRRLIB_FindGroup(GRRLIB_Model* model, char* name)
{
  GRRLIB_Group* group;

  group = model->groups;
  while(group) {
    if (!strcmp(name, group->name))
      break;
    group = group->next;
  }

  return group;
}

/**
 * Add a group to the model.
 * @param model Structure that defines the model to wich the group will be added.
 * @param name Yhe name of the group to add.
 */
static GRRLIB_Group* GRRLIB_AddGroup(GRRLIB_Model* model, char* name)
{
    GRRLIB_Group* group;

    group = GRRLIB_FindGroup(model, name);
    if (!group) {
        group = (GRRLIB_Group*)malloc(sizeof(GRRLIB_Group));
        group->name = strdup(name);
        group->material = 0;
        group->numtriangles = 0;
        group->triangles = NULL;
        group->next = model->groups;
        model->groups = group;
        model->numgroups++;
    }

    return group;
}

static void GRRLIB_SecondPass(GRRLIB_Model* model, FILE* file)
{
    u32    numvertices;		/* number of vertices in model */
    u32    numnormals;			/* number of normals in model */
    u32    numtexcoords;		/* number of texcoords in model */
    u32    numtriangles;		/* number of triangles in model */
    f32*  vertices;			/* array of vertices  */
    f32*  normals;			/* array of normals */
    f32*  texcoords;			/* array of texture coordinates */
    GRRLIB_Group* group;			/* current group pointer */
    u32    material;			/* current material */
    u32    v, n, t;
    char   buf[128];

    // set the pointer shortcuts
    vertices     = model->vertices;
    normals      = model->normals;
    texcoords    = model->texcoords;
    group        = model->groups;

    /* on the second pass through the file, read all the data into the
     allocated arrays */
    numvertices = numnormals = numtexcoords = 1;
    numtriangles = 0;
    material = 0;
    while(fscanf(file, "%s", buf) != EOF) {
        switch(buf[0]) {
            case '#':				/* comment */
                /* eat up rest of line */
                fgets(buf, sizeof(buf), file);
            break;
            case 'v':				/* v, vn, vt */
                switch(buf[1]) {
                    case '\0':			/* vertex */
                        fscanf(file, "%f %f %f",
                           &vertices[3 * numvertices + X],
                           &vertices[3 * numvertices + Y],
                           &vertices[3 * numvertices + Z]);
                        numvertices++;
                    break;
                    case 'n':				/* normal */
                        fscanf(file, "%f %f %f",
                           &normals[3 * numnormals + X],
                           &normals[3 * numnormals + Y],
                           &normals[3 * numnormals + Z]);
                        numnormals++;
                    break;
                    case 't':				/* texcoord */
                        fscanf(file, "%f %f",
                           &texcoords[2 * numtexcoords + X],
                           &texcoords[2 * numtexcoords + Y]);
                        numtexcoords++;
                    break;
                }
            break;
            case 'u':
                fgets(buf, sizeof(buf), file);
                sscanf(buf, "%s %s", buf, buf);
                //group->material = material = _glmFindMaterial(model, buf);
            break;
            case 'g':				/* group */
                /* eat up rest of line */
                fgets(buf, sizeof(buf), file);
                sscanf(buf, "%s", buf);
                group = GRRLIB_AddGroup(model, buf);
                group->material = material;
            break;
            case 'f':				/* face */
                v = n = t = 0;
                fscanf(file, "%s", buf);
                // can be one of %d, %d//%d, %d/%d, %d/%d/%d %d//%d
                if (strstr(buf, "//")) {
                    // v//n
                    sscanf(buf, "%d//%d", &v, &n);
                    T(numtriangles).vindices[0] = v;
                    T(numtriangles).nindices[0] = n;
                    fscanf(file, "%d//%d", &v, &n);
                    T(numtriangles).vindices[1] = v;
                    T(numtriangles).nindices[1] = n;
                    fscanf(file, "%d//%d", &v, &n);
                    T(numtriangles).vindices[2] = v;
                    T(numtriangles).nindices[2] = n;
                    group->triangles[group->numtriangles++] = numtriangles;
                    numtriangles++;
                    while(fscanf(file, "%d//%d", &v, &n) > 0) {
                        T(numtriangles).vindices[0] = T(numtriangles-1).vindices[0];
                        T(numtriangles).nindices[0] = T(numtriangles-1).nindices[0];
                        T(numtriangles).vindices[1] = T(numtriangles-1).vindices[2];
                        T(numtriangles).nindices[1] = T(numtriangles-1).nindices[2];
                        T(numtriangles).vindices[2] = v;
                        T(numtriangles).nindices[2] = n;
                        group->triangles[group->numtriangles++] = numtriangles;
                        numtriangles++;
                    }
            	} else if (sscanf(buf, "%d/%d/%d", &v, &t, &n) == 3) {
                    // v/t/n
                    T(numtriangles).vindices[0] = v;
                    T(numtriangles).tindices[0] = t;
                    T(numtriangles).nindices[0] = n;
                    fscanf(file, "%d/%d/%d", &v, &t, &n);
                    T(numtriangles).vindices[1] = v;
                    T(numtriangles).tindices[1] = t;
                    T(numtriangles).nindices[1] = n;
                    fscanf(file, "%d/%d/%d", &v, &t, &n);
                    T(numtriangles).vindices[2] = v;
                    T(numtriangles).tindices[2] = t;
                    T(numtriangles).nindices[2] = n;
                    group->triangles[group->numtriangles++] = numtriangles;
                    numtriangles++;
                    while(fscanf(file, "%d/%d/%d", &v, &t, &n) > 0) {
                        T(numtriangles).vindices[0] = T(numtriangles-1).vindices[0];
                        T(numtriangles).tindices[0] = T(numtriangles-1).tindices[0];
                        T(numtriangles).nindices[0] = T(numtriangles-1).nindices[0];
                        T(numtriangles).vindices[1] = T(numtriangles-1).vindices[2];
                        T(numtriangles).tindices[1] = T(numtriangles-1).tindices[2];
                        T(numtriangles).nindices[1] = T(numtriangles-1).nindices[2];
                        T(numtriangles).vindices[2] = v;
                        T(numtriangles).tindices[2] = t;
                        T(numtriangles).nindices[2] = n;
                        group->triangles[group->numtriangles++] = numtriangles;
                        numtriangles++;
                    }
                } else if (sscanf(buf, "%d/%d", &v, &t) == 2) {
                    // v/t
                    T(numtriangles).vindices[0] = v;
                    T(numtriangles).tindices[0] = t;
                    fscanf(file, "%d/%d", &v, &t);
                    T(numtriangles).vindices[1] = v;
                    T(numtriangles).tindices[1] = t;
                    fscanf(file, "%d/%d", &v, &t);
                    T(numtriangles).vindices[2] = v;
                    T(numtriangles).tindices[2] = t;
                    group->triangles[group->numtriangles++] = numtriangles;
                    numtriangles++;
                    while(fscanf(file, "%d/%d", &v, &t) > 0) {
                        T(numtriangles).vindices[0] = T(numtriangles-1).vindices[0];
                        T(numtriangles).tindices[0] = T(numtriangles-1).tindices[0];
                        T(numtriangles).vindices[1] = T(numtriangles-1).vindices[2];
                        T(numtriangles).tindices[1] = T(numtriangles-1).tindices[2];
                        T(numtriangles).vindices[2] = v;
                        T(numtriangles).tindices[2] = t;
                        group->triangles[group->numtriangles++] = numtriangles;
                        numtriangles++;
                	}
                } else {
                    // v
                    sscanf(buf, "%d", &v);
                    T(numtriangles).vindices[0] = v;
                    fscanf(file, "%d", &v);
                    T(numtriangles).vindices[1] = v;
                    fscanf(file, "%d", &v);
                    T(numtriangles).vindices[2] = v;
                    group->triangles[group->numtriangles++] = numtriangles;
                    numtriangles++;
                    while(fscanf(file, "%d", &v) > 0) {
                        T(numtriangles).vindices[0] = T(numtriangles-1).vindices[0];
                        T(numtriangles).vindices[1] = T(numtriangles-1).vindices[2];
                        T(numtriangles).vindices[2] = v;
                        group->triangles[group->numtriangles++] = numtriangles;
                        numtriangles++;
                    }
                }
            break;
            default:
                /* eat up rest of line */
                fgets(buf, sizeof(buf), file);
            break;
    	}
    }
}

static void GRRLIB_FirstPass(GRRLIB_Model* model, FILE* file)
{
    u32    numvertices;		/* number of vertices in model */
    u32    numnormals;		/* number of normals in model */
    u32    numtexcoords;		/* number of texcoords in model */
    u32    numtriangles;		/* number of triangles in model */
    GRRLIB_Group* group;			/* current group */
    unsigned  v, n, t;
    char      buf[128];

    // make a default group
    group = GRRLIB_AddGroup(model, "default");

    numvertices = numnormals = numtexcoords = numtriangles = 0;

    while(fscanf(file, "%s", buf) != EOF) {
        switch(buf[0]) {
            case '#':				/* comment */
                fgets(buf, sizeof(buf), file);
                break;
            case 'v':				/* v, vn, vt */
                switch(buf[1]) {
                    case '\0':			/* vertex */
                        fgets(buf, sizeof(buf), file);
                        numvertices++;
                        break;
                    case 'n':				/* normal */
                        fgets(buf, sizeof(buf), file);
                        numnormals++;
                        break;
                    case 't':				/* texcoord */
                        fgets(buf, sizeof(buf), file);
                        numtexcoords++;
                        break;
                    default:
                        exit(1);
                }
                break;
            case 'm':
                fgets(buf, sizeof(buf), file);
                sscanf(buf, "%s %s", buf, buf);
                model->mtllibname = strdup(buf);
                //_glmReadMTL(model, buf);
                break;
            case 'u':
                fgets(buf, sizeof(buf), file);
                break;
            case 'g':				/* group */
                fgets(buf, sizeof(buf), file);
                sscanf(buf, "%s", buf);
                group = GRRLIB_AddGroup(model, buf);
                break;
            case 'f':				/* face */
                v = n = t = 0;
                fscanf(file, "%s", buf);
                /* can be one of %d, %d//%d, %d/%d, %d/%d/%d %d//%d */
                if (strstr(buf, "//")) {
                    /* v//n */
                    sscanf(buf, "%d//%d", &v, &n);
                    fscanf(file, "%d//%d", &v, &n);
                    fscanf(file, "%d//%d", &v, &n);
                    numtriangles++;
                    group->numtriangles++;
                    while(fscanf(file, "%d//%d", &v, &n) > 0) {
                        numtriangles++;
                        group->numtriangles++;
                    }
                }
                else if (sscanf(buf, "%d/%d/%d", &v, &t, &n) == 3) {
                    /* v/t/n */
                    fscanf(file, "%d/%d/%d", &v, &t, &n);
                    fscanf(file, "%d/%d/%d", &v, &t, &n);
                    numtriangles++;
                    group->numtriangles++;
                    while(fscanf(file, "%d/%d/%d", &v, &t, &n) > 0) {
                        numtriangles++;
                        group->numtriangles++;
                    }
                }
                else if (sscanf(buf, "%d/%d", &v, &t) == 2) {
                    /* v/t */
                    fscanf(file, "%d/%d", &v, &t);
                    fscanf(file, "%d/%d", &v, &t);
                    numtriangles++;
                    group->numtriangles++;
                    while(fscanf(file, "%d/%d", &v, &t) > 0) {
                        numtriangles++;
                        group->numtriangles++;
                    }
                }
                else {
                    /* v */
                    fscanf(file, "%d", &v);
                    fscanf(file, "%d", &v);
                    numtriangles++;
                    group->numtriangles++;
                    while(fscanf(file, "%d", &v) > 0) {
                        numtriangles++;
                        group->numtriangles++;
                    }
                }
                break;
            default:
                fgets(buf, sizeof(buf), file);
                break;
        }
    }

    model->numvertices  = numvertices;
    model->numnormals   = numnormals;
    model->numtexcoords = numtexcoords;
    model->numtriangles = numtriangles;

    // allocate memory for the triangles in each group
    group = model->groups;
    while(group) {
        group->triangles = (u32*)malloc(sizeof(u32) * group->numtriangles);
        group->numtriangles = 0;
        group = group->next;
    }
}

GRRLIB_Model* GRRLIB_ReadOBJ(char* filename)
{
    GRRLIB_Model* model;
    FILE*     file;

    file = fopen(filename, "r");
    if (!file) {
        exit(1);
    }

    model = (GRRLIB_Model*)malloc(sizeof(GRRLIB_Model));
    model->pathname      = strdup(filename);
    model->mtllibname    = NULL;
    model->numvertices   = 0;
    model->vertices      = NULL;
    model->numnormals    = 0;
    model->normals       = NULL;
    model->numtexcoords  = 0;
    model->texcoords     = NULL;
    model->numfacetnorms = 0;
    model->facetnorms    = NULL;
    model->numtriangles  = 0;
    model->triangles     = NULL;
    model->nummaterials  = 0;
    //model->materials     = NULL;
    model->numgroups     = 0;
    model->groups        = NULL;
    model->position[0]   = 0.0;
    model->position[1]   = 0.0;
    model->position[2]   = 0.0;

    // Get a count of the number of stuff
    GRRLIB_FirstPass(model, file);

    // allocate memory
    model->vertices = (f32*)malloc(sizeof(f32) * 3 * (model->numvertices + 1));
    model->triangles = (GRRLIB_Triangle*)malloc(sizeof(GRRLIB_Triangle) * model->numtriangles);
    if (model->numnormals) {
        model->normals = (f32*)malloc(sizeof(f32) * 3 * (model->numnormals + 1));
    }
    if (model->numtexcoords) {
        model->texcoords = (f32*)malloc(sizeof(f32) * 2 * (model->numtexcoords + 1));
    }

    rewind(file);

    GRRLIB_SecondPass(model, file);

    fclose(file);

    return model;
}

/**
 * Deletes a GRRLIB_Model structure.
 * @param model Initialized GRRLIB_Model structure.
 */
void GRRLIB_DeleteObj(GRRLIB_Model* model)
{
    GRRLIB_Group* group;
    u32 i;

    if (model->pathname)   free(model->pathname);
    if (model->mtllibname) free(model->mtllibname);
    if (model->vertices)   free(model->vertices);
    if (model->normals)    free(model->normals);
    if (model->texcoords)  free(model->texcoords);
    if (model->facetnorms) free(model->facetnorms);
    if (model->triangles)  free(model->triangles);
    /*
    if (model->materials) {
        for (i = 0; i < model->nummaterials; i++)
            free(model->materials[i].name);
    }
    free(model->materials);
    */
    while(model->groups) {
        group = model->groups;
        model->groups = model->groups->next;
        free(group->name);
        free(group->triangles);
        free(group);
    }

    free(model);
}

/**
 * Draw a 3D object.
 * @param model Structure that defines the model to draw.
 */
void Draw3dObj(GRRLIB_Model* model)
{
    GRRLIB_Group* group;
    int i;

    group = model->groups;
    while (group) {
        GX_Begin(GX_TRIANGLES, GX_VTXFMT0, group->numtriangles*3);
        for (i = 0; i < group->numtriangles; i++) {
            GX_Position3f32(model->vertices[3 * T(group->triangles[i]).vindices[0] + X],
                            model->vertices[3 * T(group->triangles[i]).vindices[0] + Y],
                            model->vertices[3 * T(group->triangles[i]).vindices[0] + Z]);
            GX_Color1u32(0xFFFFFFFF);
            if(model->numtexcoords) {
                GX_TexCoord2f32(model->texcoords[2*T(group->triangles[i]).tindices[0] + X],
                                model->texcoords[2*T(group->triangles[i]).tindices[0] + Y]);
            }
            if(model->numnormals) {
                GX_Normal3f32(model->normals[3 * T(group->triangles[i]).nindices[0] + X],
                              model->normals[3 * T(group->triangles[i]).nindices[0] + Y],
                              model->normals[3 * T(group->triangles[i]).nindices[0] + Z]);
            }



            GX_Position3f32(model->vertices[3 * T(group->triangles[i]).vindices[1] + X],
                            model->vertices[3 * T(group->triangles[i]).vindices[1] + Y],
                            model->vertices[3 * T(group->triangles[i]).vindices[1] + Z]);
            GX_Color1u32(0xFFFFFFFF);
            if(model->numtexcoords) {
                GX_TexCoord2f32(model->texcoords[2*T(group->triangles[i]).tindices[1] + X],
                                model->texcoords[2*T(group->triangles[i]).tindices[1] + Y]);
            }
            if(model->numnormals) {
                GX_Normal3f32(model->normals[3 * T(group->triangles[i]).nindices[1] + X],
                              model->normals[3 * T(group->triangles[i]).nindices[1] + Y],
                              model->normals[3 * T(group->triangles[i]).nindices[1] + Z]);
            }



            GX_Position3f32(model->vertices[3 * T(group->triangles[i]).vindices[2] + X],
                            model->vertices[3 * T(group->triangles[i]).vindices[2] + Y],
                            model->vertices[3 * T(group->triangles[i]).vindices[2] + Z]);
            GX_Color1u32(0xFFFFFFFF);
            if(model->numtexcoords) {
                GX_TexCoord2f32(model->texcoords[2*T(group->triangles[i]).tindices[2] + X],
                                model->texcoords[2*T(group->triangles[i]).tindices[2] + Y]);
            }
            if(model->numnormals) {
                GX_Normal3f32(model->normals[3 * T(group->triangles[i]).nindices[2] + X],
                              model->normals[3 * T(group->triangles[i]).nindices[2] + Y],
                              model->normals[3 * T(group->triangles[i]).nindices[2] + Z]);
            }
        }
        GX_End();
        group = group->next;
    }
}



















static void GRRLIB_FirstPassMem(GRRLIB_Model* model, char *buffer, u32 size)
{
    u32    numvertices;		/* number of vertices in model */
    u32    numnormals;		/* number of normals in model */
    u32    numtexcoords;		/* number of texcoords in model */
    u32    numtriangles;		/* number of triangles in model */
    GRRLIB_Group* group;			/* current group */
    unsigned  v, n, t;
    char      buf[128];
    int ss;
/*
    char *buffer;
    buffer = (char*)malloc(sizeof(char) * size);
    memcpy(buffer, buffer_o, size);
*/
    // make a default group
    group = GRRLIB_AddGroup(model, "default");

    numvertices = numnormals = numtexcoords = numtriangles = 0;

    while((ss = sscanf(buffer, "%s", buf)) != EOF) {
        buffer += (ss + 1);
        switch(buf[0]) {
            case '#':				/* comment */
                buffer = strchr(buffer, '\n') + 1;
                break;
            case 'v':				/* v, vn, vt */
                switch(buf[1]) {
                    case '\0':			/* vertex */
                        numvertices++;
                        break;
                    case 'n':				/* normal */
                        numnormals++;
                        break;
                    case 't':				/* texcoord */
                        numtexcoords++;
                        break;
                    default:
                        exit(1);
                }
                buffer = strchr(buffer, '\n') + 1;
                break;
            case 'm':
                buffer = strchr(buffer, '\n') + 1;
                sscanf(buf, "%s %s", buf, buf);
                model->mtllibname = strdup(buf);
                //_glmReadMTL(model, buf);
                break;
            case 'u':
                buffer = strchr(buffer, '\n') + 1;
                break;
            case 'g':				/* group */
                buffer = strchr(buffer, '\n') + 1;
                sscanf(buf, "%s", buf);
                group = GRRLIB_AddGroup(model, buf);
                break;
            case 'f':				/* face */
                v = n = t = 0;
                sscanf(buffer, "%s", buf);
                /* can be one of %d, %d//%d, %d/%d, %d/%d/%d %d//%d */
                if (strstr(buf, "//")) {
                    /* v//n */
                    sscanf(buf, "%d//%d", &v, &n);
                    buffer = strchr(buffer, 32) + 1;
                    sscanf(buffer, "%d//%d", &v, &n);
                    buffer = strchr(buffer, 32) + 1;
                    sscanf(buffer, "%d//%d", &v, &n);
                    buffer = strchr(buffer, 32);
                    numtriangles++;
                    group->numtriangles++;
                    while(sscanf(buffer, "%d//%d", &v, &n) > 0) {
                        buffer = strchr(buffer, 32) + 1;
                        numtriangles++;
                        group->numtriangles++;
                    }
                }
                else if (sscanf(buf, "%d/%d/%d", &v, &t, &n) == 3) {
                    /* v/t/n */
                    sscanf(buffer, "%d/%d/%d", &v, &t, &n);
                    sscanf(buffer, "%d/%d/%d", &v, &t, &n);
                    numtriangles++;
                    group->numtriangles++;
                    while(sscanf(buffer, "%d/%d/%d", &v, &t, &n) > 0) {
                        numtriangles++;
                        group->numtriangles++;
                    }
                }
                else if (sscanf(buf, "%d/%d", &v, &t) == 2) {
                    /* v/t */
                    sscanf(buffer, "%d/%d", &v, &t);
                    sscanf(buffer, "%d/%d", &v, &t);
                    numtriangles++;
                    group->numtriangles++;
                    while(sscanf(buffer, "%d/%d", &v, &t) > 0) {
                        numtriangles++;
                        group->numtriangles++;
                    }
                }
                else {
                    /* v */
                    sscanf(buffer, "%d", &v);
                    sscanf(buffer, "%d", &v);
                    numtriangles++;
                    group->numtriangles++;
                    while(sscanf(buffer, "%d", &v) > 0) {
                        numtriangles++;
                        group->numtriangles++;
                    }
                }
                break;
            default:
                buffer = strchr(buffer, '\n') + 1;
                break;
        }
        if(*buffer == '\n') {
            buffer++;
        }
    }

    model->numvertices  = numvertices;
    model->numnormals   = numnormals;
    model->numtexcoords = numtexcoords;
    model->numtriangles = numtriangles;

    // allocate memory for the triangles in each group
    group = model->groups;
    while(group) {
        group->triangles = (u32*)malloc(sizeof(u32) * group->numtriangles);
        group->numtriangles = 0;
        group = group->next;
    }
}

GRRLIB_Model* GRRLIB_ReadOBJMem(const char *buffer, u32 size)
{
    GRRLIB_Model* model;

    model = (GRRLIB_Model*)malloc(sizeof(GRRLIB_Model));
    model->pathname      = "MEMORY";
    model->mtllibname    = NULL;
    model->numvertices   = 0;
    model->vertices      = NULL;
    model->numnormals    = 0;
    model->normals       = NULL;
    model->numtexcoords  = 0;
    model->texcoords     = NULL;
    model->numfacetnorms = 0;
    model->facetnorms    = NULL;
    model->numtriangles  = 0;
    model->triangles     = NULL;
    model->nummaterials  = 0;
    //model->materials     = NULL;
    model->numgroups     = 0;
    model->groups        = NULL;
    model->position[0]   = 0.0;
    model->position[1]   = 0.0;
    model->position[2]   = 0.0;

    // Get a count of the number of stuff
    GRRLIB_FirstPassMem(model, buffer, size);

    // allocate memory
    model->vertices = (f32*)malloc(sizeof(f32) * 3 * (model->numvertices + 1));
    model->triangles = (GRRLIB_Triangle*)malloc(sizeof(GRRLIB_Triangle) * model->numtriangles);
    if (model->numnormals) {
        model->normals = (f32*)malloc(sizeof(f32) * 3 * (model->numnormals + 1));
    }
    if (model->numtexcoords) {
        model->texcoords = (f32*)malloc(sizeof(f32) * 2 * (model->numtexcoords + 1));
    }

    //GRRLIB_SecondPassMem(model, buffer, size);

    return model;
}