#pragma once
#include "../GraphicsApi.h"
#include <string>

class GLApi : public GraphicsApiI {
public:
   GLApi();
   bool init() override;
   void terminate() override;

   ShaderI* createShader(GraphicsApiI::ShaderType shaderType) override;
   ShaderProgramI* createShaderProgram() override;
   BufferI* createBuffer(GraphicsApiI::BufferType bufferType = GraphicsApiI::BufferType::VERTEX) override;
   VertexLayoutI* createVertexLayout() override;
   ImageI* createImage(GraphicsApiI::TextureType texType = GraphicsApiI::TextureType::TEX_2D) override;

   void setViewport(glm::ivec2 pos, glm::ivec2 size) override;
   void clearColor(glm::vec4 color) override;
   void clear(ClearType clearFlags) override;
   static int getCombinedMaxTextureBindings();
   ~GLApi() override;

private:
   //Clear
   static unsigned int getGlClearType(GraphicsApiI::ClearType clearType);
   static unsigned int getGlShaderType(GraphicsApiI::ShaderType shaderType);
   static unsigned int getGlBufferType(GraphicsApiI::BufferType bufferType);
   static unsigned int getGlBufferUsageType(GraphicsApiI::BufferUsageType bufferUsageType);
   static unsigned int getGlBufferAccessType(GraphicsApiI::BufferAccessType bufferAccessType);
   static unsigned int getGlDataType(GraphicsApiI::ApiDataType dataType);
   static unsigned int getGlTexFormatType(GraphicsApiI::TexFormatType texFormatType);
   static unsigned int getGlTextureType(GraphicsApiI::TextureType textureType);

   friend class GLShader;
   friend class GLShaderProgram;
   friend class GLBuffer;
   friend class GLVAL;
   friend class GLImage;
};

class GLShader : public ShaderI {
public:
   GLShader(GraphicsApiI::ShaderType shaderType);
   bool compile(std::string shaderSrc) override;
   std::string getError() override;
   ~GLShader() override;

   unsigned int shaderID;
private:
   friend class GLShaderProgram;
};

class GLShaderProgram : public ShaderProgramI {
public:
   GLShaderProgram();
   bool attachShader(ShaderI* shader) override;
   bool linkProgram() override;
   std::string getError() override;

   int getUniformLocation(const std::string& name);

   void setUniformImpl(const std::string& varName, const int* values, int lengt) override;
   void setUniformImpl(const std::string& varName, const unsigned int* values, int lengt) override;
   void setUniformImpl(const std::string& varName, const float* values, int lengt) override;
   void setUniformImpl(const std::string& varName, const double* values, int lengt) override;

   //glm vector
   void setUniformImpl(const std::string& varName, const glm::ivec2* values, int lengt) override;
   void setUniformImpl(const std::string& varName, const glm::ivec3* values, int lengt) override;
   void setUniformImpl(const std::string& varName, const glm::ivec4* values, int lengt) override;

   void setUniformImpl(const std::string& varName, const glm::uvec2* values, int lengt) override;
   void setUniformImpl(const std::string& varName, const glm::uvec3* values, int lengt) override;
   void setUniformImpl(const std::string& varName, const glm::uvec4* values, int lengt) override;

   void setUniformImpl(const std::string& varName, const glm::vec2* values, int lengt) override;
   void setUniformImpl(const std::string& varName, const glm::vec3* values, int lengt) override;
   void setUniformImpl(const std::string& varName, const glm::vec4* values, int lengt) override;

   void setUniformImpl(const std::string& varName, const glm::dvec2* values, int lengt) override;
   void setUniformImpl(const std::string& varName, const glm::dvec3* values, int lengt) override;
   void setUniformImpl(const std::string& varName, const glm::dvec4* values, int lengt) override;

   //glm matrix
   void setUniformImpl(const std::string& varName, const glm::mat2* values, int lengt) override;
   void setUniformImpl(const std::string& varName, const glm::mat3* values, int lengt) override;
   void setUniformImpl(const std::string& varName, const glm::mat4* values, int lengt) override;

   void setUniformImpl(const std::string& varName, const glm::dmat2* values, int lengt) override;
   void setUniformImpl(const std::string& varName, const glm::dmat3* values, int lengt) override;
   void setUniformImpl(const std::string& varName, const glm::dmat4* values, int lengt) override;

   void setUniformImpl(const std::string& varName, const ImageI* values, int length) override;

   void useProgram() override;
   ~GLShaderProgram() override;

   unsigned int shaderProgram;
};

class GLBuffer : public BufferI {
public:
   GLBuffer(GraphicsApiI::BufferType bufferType = GraphicsApiI::BufferType::VERTEX);
   void setType(GraphicsApiI::BufferType bufferType) override;
   void bind() override;
   void unbind() override;
   void* getData(GraphicsApiI::BufferAccessType bufferAccessType) override;
   void setData(const void* data, int size, GraphicsApiI::BufferUsageType bufferUsageType = GraphicsApiI::BufferUsageType::STATIC) override;
   ~GLBuffer() override;

   unsigned int buffer;
   GraphicsApiI::BufferType type;
};

class GLVAL : public VertexLayoutI {
public:
   void addBuffer(BufferI* buffer) override;
   void addAttribLayout(GraphicsApiI::VertexLayoutAttrib rule) override;
   void bind() override;
   void unbind() override;
   ~GLVAL() override;

   std::vector<BufferI*> buffers;
   std::vector<GraphicsApiI::VertexLayoutAttrib> rules;
};

class GLImage : public ImageI {
public:
   GLImage(GraphicsApiI::TextureType texType = GraphicsApiI::TextureType::TEX_2D);
   void setType(GraphicsApiI::TextureType texType = GraphicsApiI::TextureType::TEX_2D) override;
   bool setChannel(unsigned int channelID) override;

   bool setData(const void* data, int size, GraphicsApiI::ApiDataType dataType, GraphicsApiI::TexFormatType texFormat, 
      unsigned int mipLevel = 0, GraphicsApiI::TexFormatType externalTexFormat = GraphicsApiI::TexFormatType::UNDEFINED) override;

   bool setData(const void* data, glm::ivec2 size, GraphicsApiI::ApiDataType dataType, GraphicsApiI::TexFormatType texFormat, 
      unsigned int mipLevel = 0, GraphicsApiI::TexFormatType externalTexFormat = GraphicsApiI::TexFormatType::UNDEFINED) override;

   bool setData(const void* data, glm::ivec3 size, GraphicsApiI::ApiDataType dataType, GraphicsApiI::TexFormatType texFormat, 
      unsigned int mipLevel = 0, GraphicsApiI::TexFormatType externalTexFormat = GraphicsApiI::TexFormatType::UNDEFINED) override;

   void genMips() override;
   void bind() override;
   void unbind() override;
   ~GLImage() override;

   unsigned int texture;
   unsigned int textureChannel = 0;
   GraphicsApiI::TextureType type;
};