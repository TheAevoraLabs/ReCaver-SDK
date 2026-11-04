#ifndef CAVER_VERTEXARRAYOBJECT_H
#define CAVER_VERTEXARRAYOBJECT_H

#include "RenderingContext.h"
#include "../model/MeshData.h"

namespace Caver {

struct ArrayDescriptor {
    int target;
    unsigned int size;
    DataType type;
    unsigned int stride;
    int offset;
    ArrayDescriptor* next;
};

class VertexArrayObject {
public:
    VertexArrayObject();
    ~VertexArrayObject();

    void Reset();
    void CreateVertexBuffer(int count);

    void* VertexData() const { return m_vertexData; }
    void* IndexData() const { return m_indexData; }

    void CreateIndexBuffer(DataType type, int count);
    void CreateBufferObjects(bool doubleBuffered);
    void AddArray(unsigned int index, int size, DataType type, int stride, void* pointer);
    void AddArray(unsigned int index, int size, DataType type, int stride, int offset);
    void SwapBufferObjects();
    void DeleteClientSideBuffers();
    void UpdateVertexBufferObjectData();
    void UpdateVertexBufferObjectSubData(int offset, int size);
    void BindArrays(RenderingContext* context);
    void UnbindArrays(RenderingContext* context);
    void Draw(RenderingContext* context, int mode, int first, int count);

    static void SetVertexAttribPointer(RenderingContext* context,
                                       unsigned int index, int size,
                                       DataType type, int stride,
                                       const void* pointer);
    static void SetVertexAttribArrayEnabled(RenderingContext* context,
                                            unsigned int array, bool enabled);

    int m_vertexCount;               // +0
    int _pad4;                       // +4
    void* m_vertexData;              // +8
    DataType m_indexType;            // +16
    int m_indexCount;                // +20
    void* m_indexData;               // +24
    int m_bufferCount;               // +32
    int m_activeBufferIndex;         // +36
    unsigned int m_vbo[2];           // +40
    unsigned int m_ibo;              // +48
    unsigned int _pad52;             // +52
    ArrayDescriptor* m_firstArray;   // +56
    unsigned int m_vao[2];           // +64
};

static_assert(sizeof(VertexArrayObject) == 72, "VertexArrayObject size must be 72");

} // namespace Caver

#endif // CAVER_VERTEXARRAYOBJECT_H
