#pragma once
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/matrix.hpp>
#include <string>
#include <vector>

class ImageI;
class ShaderI;
class ShaderProgramI;
class BufferI;
class VertexLayoutI;
class ImageI;

class GraphicsApiI {
public:
   enum class ClearType {
      UNDEFINED = 0,
      COLOR = 1 << 0,
      DEPTH = 1 << 1
   };

   enum class ShaderType {
      UNDEFINED = 0,
      VERTEX,
      FRAGMENT,
      GEOMETRY
   };

   enum class BufferType {
      UNDEFINED = 0,
      VERTEX = 1 << 0,
      INDEX = 1 << 1,
      UNIFORM = 1 << 2,
      STORAGE = 1 << 3
   };

   enum class BufferUsageType {
      UNDEFINED = 0,
      STATIC,
      DYNAMIC,
      STREAM
   };

   enum class BufferAccessType {
      UNDEFINED = 0,
      READ,
      WRITE,
      READ_WRITE
   };

   enum class ApiDataType {
      UDEFINED = 0,
      BYTE,
      UBYTE,
      UINT,
      INT,
      FLOAT,
      DOUBLE
   };

   enum class TexFormatType {
      UNDEFINED = 0,
      RGBA,
      RGB
   };

   enum class TextureType {
      TEX_1D,
      TEX_2D,
      TEX_3D
   };

   struct VertexLayoutAttrib {
      VertexLayoutAttrib(int bufferID, int attribID, unsigned int dimmension, ApiDataType attribType, unsigned int bufferElementSize = 1, unsigned int offset = 0) 
      : bufferID(bufferID), attribID(attribID), dimmension(dimmension), attribType(attribType), bufferElementSize(bufferElementSize), offset(offset) {}

      int bufferID;
      int attribID;
      unsigned int dimmension;
      ApiDataType attribType;
      unsigned int bufferElementSize = 1;
      unsigned int offset = 0;
   };

   virtual bool init() = 0;
   virtual void terminate() = 0;

   virtual ShaderI* createShader(GraphicsApiI::ShaderType shaderType) = 0;
   virtual ShaderProgramI* createShaderProgram() = 0;
   virtual BufferI* createBuffer(GraphicsApiI::BufferType bufferType = GraphicsApiI::BufferType::VERTEX) = 0;
   virtual VertexLayoutI* createVertexLayout() = 0;
   virtual ImageI* createImage(GraphicsApiI::TextureType texType = GraphicsApiI::TextureType::TEX_2D) = 0;

   virtual void setViewport(glm::ivec2 pos, glm::ivec2 size) = 0;
   virtual void clearColor(glm::vec4 color) = 0;
   virtual void clear(ClearType clearFlags) = 0;

   static int getCombinedMaxTextureBindings() {return 0;};
   virtual ~GraphicsApiI() = default;
};

inline GraphicsApiI::ClearType operator|(GraphicsApiI::ClearType a, GraphicsApiI::ClearType b) {
   return (GraphicsApiI::ClearType)((int)a | (int)b);
}

inline GraphicsApiI::ClearType operator&(GraphicsApiI::ClearType a, GraphicsApiI::ClearType b) {
   return (GraphicsApiI::ClearType)((int)a & (int)b);
}

inline GraphicsApiI::ClearType operator~(GraphicsApiI::ClearType a) {
   return (GraphicsApiI::ClearType)(~(int)a);
}

inline bool hasFlag(GraphicsApiI::ClearType flags, GraphicsApiI::ClearType flag) {
   return ((int)flags & (int)flag) != 0;
}

class ShaderI {
public:
   virtual bool compile(std::string shaderSrc) = 0;
   virtual std::string getError() = 0;
   virtual ~ShaderI() = default;
};

class ShaderProgramI {
public:
   virtual bool attachShader(ShaderI* shader) = 0;
   virtual bool linkProgram() = 0;
   virtual std::string getError() = 0;
   virtual void useProgram() = 0;

   template<typename T>
   void setUniform(const std::string& name, const T& value) {
      setUniformImpl(name, &value, 1);
   }

   template<typename T>
   void setUniform(const std::string& name, const std::vector<T>& values) {
      if (values.empty()) return;

      setUniformImpl(name, values.data(), values.size());
   }

   template<typename T, int N>
   void setUniform(const std::string& name, const T(&values)[N]) {
      setUniformImpl(name, values, N);
   }

   template<typename T>
   void setUniform(const std::string& name, const T* values, int N) {
      setUniformImpl(name, values, N);
   }

   virtual ~ShaderProgramI() = default;
protected:
   virtual void setUniformImpl(const std::string&, const int*, int) = 0;
   virtual void setUniformImpl(const std::string&, const unsigned int*, int) = 0;
   virtual void setUniformImpl(const std::string&, const float*, int) = 0;
   virtual void setUniformImpl(const std::string&, const double*, int) = 0;

   //glm vector
   virtual void setUniformImpl(const std::string&, const glm::ivec2*, int) = 0;
   virtual void setUniformImpl(const std::string&, const glm::ivec3*, int) = 0;
   virtual void setUniformImpl(const std::string&, const glm::ivec4*, int) = 0;

   virtual void setUniformImpl(const std::string&, const glm::uvec2*, int) = 0;
   virtual void setUniformImpl(const std::string&, const glm::uvec3*, int) = 0;
   virtual void setUniformImpl(const std::string&, const glm::uvec4*, int) = 0;

   virtual void setUniformImpl(const std::string&, const glm::vec2*, int) = 0;
   virtual void setUniformImpl(const std::string&, const glm::vec3*, int) = 0;
   virtual void setUniformImpl(const std::string&, const glm::vec4*, int) = 0;

   virtual void setUniformImpl(const std::string&, const glm::dvec2*, int) = 0;
   virtual void setUniformImpl(const std::string&, const glm::dvec3*, int) = 0;
   virtual void setUniformImpl(const std::string&, const glm::dvec4*, int) = 0;

   //glm matrix
   virtual void setUniformImpl(const std::string&, const glm::mat2*, int) = 0;
   virtual void setUniformImpl(const std::string&, const glm::mat3*, int) = 0;
   virtual void setUniformImpl(const std::string&, const glm::mat4*, int) = 0;

   virtual void setUniformImpl(const std::string&, const glm::dmat2*, int) = 0;
   virtual void setUniformImpl(const std::string&, const glm::dmat3*, int) = 0;
   virtual void setUniformImpl(const std::string&, const glm::dmat4*, int) = 0;

   //texture
   virtual void setUniformImpl(const std::string&, const ImageI*, int) = 0;
};

class BufferI {
public:
   virtual void setType(GraphicsApiI::BufferType bufferType) = 0;
   virtual void bind() = 0;
   virtual void unbind() = 0;

   template<typename T>
   void setData(const std::vector<T>& values, GraphicsApiI::BufferUsageType bufferUsageType = GraphicsApiI::BufferUsageType::STATIC) {
      if (values.empty()) return;
      setData(values.data(), sizeof(T)*values.size());
   }

   virtual void setData(const void* data, int size, GraphicsApiI::BufferUsageType bufferUsageType = GraphicsApiI::BufferUsageType::STATIC) = 0;
   virtual void* getData(GraphicsApiI::BufferAccessType bufferAccessType) = 0;
   virtual ~BufferI() = default;
};

class VertexLayoutI {
public:
   virtual void addBuffer(BufferI* buffer) = 0;
   virtual void addAttribLayout(GraphicsApiI::VertexLayoutAttrib rule) = 0;
   virtual void bind() = 0;
   virtual void unbind() = 0;
   virtual ~VertexLayoutI() = default;
};

class ImageI {
public:
   virtual void setType(GraphicsApiI::TextureType texType) = 0;
   virtual bool setChannel(unsigned int channelID) = 0;

   template<typename T>
   bool setData(const std::vector<T>& data, int size, GraphicsApiI::ApiDataType dataType, GraphicsApiI::TexFormatType texFormat, 
      unsigned int mipLevel = 0, GraphicsApiI::TexFormatType externalTexFormat = GraphicsApiI::TexFormatType::UNDEFINED) {
      if (data.empty()) return false;
      return setData(data.data(), size, dataType, texFormat, mipLevel, externalTexFormat);
   }

   template<typename T>
   bool setData(const std::vector<T>& data, glm::ivec2 size, GraphicsApiI::ApiDataType dataType, GraphicsApiI::TexFormatType texFormat, 
      unsigned int mipLevel = 0, GraphicsApiI::TexFormatType externalTexFormat = GraphicsApiI::TexFormatType::UNDEFINED) {
      if (data.empty()) return false;
      return setData(data.data(), size, dataType, texFormat, mipLevel, externalTexFormat);
   }

   template<typename T>
   bool setData(const std::vector<T>& data, glm::ivec3 size, GraphicsApiI::ApiDataType dataType, GraphicsApiI::TexFormatType texFormat, 
      unsigned int mipLevel = 0, GraphicsApiI::TexFormatType externalTexFormat = GraphicsApiI::TexFormatType::UNDEFINED) {
      if (data.empty()) return false;
      return setData(data.data(), size, dataType, texFormat, mipLevel, externalTexFormat);
   }

   virtual bool setData(const void* data, int size, GraphicsApiI::ApiDataType dataType, GraphicsApiI::TexFormatType texFormat, 
      unsigned int mipLevel = 0, GraphicsApiI::TexFormatType externalTexFormat = GraphicsApiI::TexFormatType::UNDEFINED) = 0;

   virtual bool setData(const void* data, glm::ivec2 size, GraphicsApiI::ApiDataType dataType, GraphicsApiI::TexFormatType texFormat, 
      unsigned int mipLevel = 0, GraphicsApiI::TexFormatType externalTexFormat = GraphicsApiI::TexFormatType::UNDEFINED) = 0;

   virtual bool setData(const void* data, glm::ivec3 size, GraphicsApiI::ApiDataType dataType, GraphicsApiI::TexFormatType texFormat, 
      unsigned int mipLevel = 0, GraphicsApiI::TexFormatType externalTexFormat = GraphicsApiI::TexFormatType::UNDEFINED) = 0;
   
   virtual void genMips() = 0;
   virtual void bind() = 0;
   virtual void unbind() = 0;
   virtual ~ImageI() = default;
};

