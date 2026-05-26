#include "GLApi.h"
#include <cstdio>
#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>
#include <algorithm>

//Api:
GLApi::GLApi() {
   init();
}

bool GLApi::init() {
   return gladLoadGL();
}

void GLApi::terminate() {
}


ShaderI* GLApi::createShader(GraphicsApiI::ShaderType shaderType) {
   GLShader* shader = new GLShader(shaderType);
   return shader;
}
ShaderProgramI* GLApi::createShaderProgram() {
   ShaderProgramI* program = new GLShaderProgram();
   return program;
}
BufferI* GLApi::createBuffer(GraphicsApiI::BufferType bufferType) {
   BufferI* buffer = new GLBuffer(bufferType);
   return buffer;
}
VertexLayoutI* GLApi::createVertexLayout() {
   VertexLayoutI* vlI = new GLVAL();
   return vlI;
}
ImageI* GLApi::createImage(GraphicsApiI::TextureType texType) {
   ImageI* image = new GLImage(texType);
   return image;
}


void GLApi::setViewport(glm::ivec2 pos, glm::ivec2 size) {
   glViewport(pos.x, pos.y, size.x, size.y);
}

void GLApi::clearColor(glm::vec4 color) {
   glClearColor(color.x, color.y, color.z, color.w);
}

void GLApi::clear(ClearType clearFlags) {
   glClear(GLApi::getGlClearType(clearFlags));
}

GLApi::~GLApi() {
   terminate();
}

int GLApi::getCombinedMaxTextureBindings() {
   int maxUnits = 0;
   glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &maxUnits);
   return maxUnits;
}

//Enum Cast:
unsigned int GLApi::getGlClearType(GraphicsApiI::ClearType clearType) {
   unsigned int clFlag = 0;
   if (hasFlag(clearType, ClearType::COLOR)) clFlag |= GL_COLOR_BUFFER_BIT;
   if (hasFlag(clearType, ClearType::DEPTH)) clFlag |= GL_DEPTH_BUFFER_BIT;
   return clFlag;
}
unsigned int GLApi::getGlShaderType(GraphicsApiI::ShaderType shaderType) {
   switch (shaderType) {
   case GraphicsApiI::ShaderType::VERTEX:
      return GL_VERTEX_SHADER;
   case GraphicsApiI::ShaderType::FRAGMENT:
      return GL_FRAGMENT_SHADER;
   case GraphicsApiI::ShaderType::GEOMETRY:
      return GL_GEOMETRY_SHADER;
   default:
      return 0;
   }
}
unsigned int GLApi::getGlBufferType(GraphicsApiI::BufferType bufferType) {
   unsigned int type = 0;
   switch (bufferType) {
   case GraphicsApiI::BufferType::VERTEX:
      return GL_ARRAY_BUFFER;
   case GraphicsApiI::BufferType::INDEX:
      return GL_ELEMENT_ARRAY_BUFFER;
   default:
      return 0;
   }
}
unsigned int GLApi::getGlBufferUsageType(GraphicsApiI::BufferUsageType bufferUsageType) {
   switch (bufferUsageType) {
   case GraphicsApiI::BufferUsageType::STATIC:
      return GL_STATIC_DRAW;
   case GraphicsApiI::BufferUsageType::DYNAMIC:
      return GL_DYNAMIC_DRAW;
   default:
      return 0;
   }
}
unsigned int GLApi::getGlBufferAccessType(GraphicsApiI::BufferAccessType bufferAccessType) {
   printf("unfinished getGlBufferAccessType\n");
   return 0;
}
unsigned int GLApi::getGlDataType(GraphicsApiI::ApiDataType dataType) {
   switch (dataType) {
   case GraphicsApiI::ApiDataType::UBYTE:
      return GL_UNSIGNED_BYTE;
   case GraphicsApiI::ApiDataType::BYTE:
      return GL_BYTE;
   case GraphicsApiI::ApiDataType::UINT:
      return GL_UNSIGNED_INT;
   case GraphicsApiI::ApiDataType::INT:
      return GL_INT;
   case GraphicsApiI::ApiDataType::FLOAT:
      return GL_FLOAT;
   case GraphicsApiI::ApiDataType::DOUBLE:
      return GL_DOUBLE;
   default:
      return 0;
   }
}
unsigned int GLApi::getGlTexFormatType(GraphicsApiI::TexFormatType texFormatType) {
   switch (texFormatType) {
   case GraphicsApiI::TexFormatType::RGB:
      return GL_RGB;
   case GraphicsApiI::TexFormatType::RGBA:
      return GL_RGBA;
   default:
      return 0;
   }
}
unsigned int GLApi::getGlTextureType(GraphicsApiI::TextureType textureType) {
   switch (textureType) {
   case GraphicsApiI::TextureType::TEX_1D:
      return GL_TEXTURE_1D;
   case GraphicsApiI::TextureType::TEX_2D:
      return GL_TEXTURE_2D;
   case GraphicsApiI::TextureType::TEX_3D:
      return GL_TEXTURE_3D;
   default:
      return 0;
   }
}


//Shader:
GLShader::GLShader(GraphicsApiI::ShaderType shaderType) {
   shaderID = glCreateShader(GLApi::getGlShaderType(shaderType));
}

bool GLShader::compile(std::string shaderSrc) {
   const char* vertSources[] = { shaderSrc.c_str() };
   glShaderSource(shaderID, 1, vertSources, NULL);
   glCompileShader(shaderID);
   int success;
   glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);
   return success;
}

std::string GLShader::getError() {
   int success;
   glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);
   if (!success) {
      std::string infoLog;
      int logLength = 0;
      glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &logLength);
      infoLog.resize(logLength);
      glGetShaderInfoLog(shaderID, logLength, NULL, infoLog.data());
      return infoLog;
   }
   return "";
}

GLShader::~GLShader() {
   glDeleteShader(shaderID);
}


GLShaderProgram::GLShaderProgram() {
   shaderProgram = glCreateProgram();
}

bool GLShaderProgram::attachShader(ShaderI* shader) {
   GLShader* shaderPtr = dynamic_cast<GLShader*>(shader);
   if (!shaderPtr) return false;
   glAttachShader(shaderProgram, shaderPtr->shaderID);
   return true;
}

bool GLShaderProgram::linkProgram() {
   glLinkProgram(shaderProgram);
   int success;
   glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
   return success;
}

std::string GLShaderProgram::getError() {
   int success;
   glGetShaderiv(shaderProgram, GL_LINK_STATUS, &success);
   if (!success) {
      std::string infoLog;
      int logLength = 0;
      glGetShaderiv(shaderProgram, GL_INFO_LOG_LENGTH, &logLength);
      infoLog.resize(logLength);
      glGetShaderInfoLog(shaderProgram, logLength, NULL, infoLog.data());
      return infoLog;
   }
   return "";
}

int GLShaderProgram::getUniformLocation(const std::string& name) {
   return glGetUniformLocation(shaderProgram, name.c_str());
}


void GLShaderProgram::setUniformImpl(const std::string& varName, const int* values, int length) {
   glUniform1iv(getUniformLocation(varName), length, values);
};
void GLShaderProgram::setUniformImpl(const std::string& varName, const unsigned int* values, int length) {
   glUniform1uiv(getUniformLocation(varName), length, values);
};
void GLShaderProgram::setUniformImpl(const std::string& varName, const float* values, int length) {
   glUniform1fv(getUniformLocation(varName), length, values);
};
void GLShaderProgram::setUniformImpl(const std::string& varName, const double* values, int length) {
   glUniform1dv(getUniformLocation(varName), length, values);
};

//glm vector
void GLShaderProgram::setUniformImpl(const std::string& varName, const glm::ivec2* values, int length) {
   glUniform2iv(getUniformLocation(varName), length, glm::value_ptr(values[0]));
};
void GLShaderProgram::setUniformImpl(const std::string& varName, const glm::ivec3* values, int length) {
   glUniform3iv(getUniformLocation(varName), length, glm::value_ptr(values[0]));
};
void GLShaderProgram::setUniformImpl(const std::string& varName, const glm::ivec4* values, int length) {
   glUniform4iv(getUniformLocation(varName), length, glm::value_ptr(values[0]));
};

void GLShaderProgram::setUniformImpl(const std::string& varName, const glm::uvec2* values, int length) {
   glUniform2uiv(getUniformLocation(varName), length, glm::value_ptr(values[0]));
};
void GLShaderProgram::setUniformImpl(const std::string& varName, const glm::uvec3* values, int length) {
   glUniform3uiv(getUniformLocation(varName), length, glm::value_ptr(values[0]));
};
void GLShaderProgram::setUniformImpl(const std::string& varName, const glm::uvec4* values, int length) {
   glUniform4uiv(getUniformLocation(varName), length, glm::value_ptr(values[0]));
};

void GLShaderProgram::setUniformImpl(const std::string& varName, const glm::vec2* values, int length) {
   glUniform2fv(getUniformLocation(varName), length, glm::value_ptr(values[0]));
};
void GLShaderProgram::setUniformImpl(const std::string& varName, const glm::vec3* values, int length) {
   glUniform3fv(getUniformLocation(varName), length, glm::value_ptr(values[0]));
};
void GLShaderProgram::setUniformImpl(const std::string& varName, const glm::vec4* values, int length) {
   glUniform4fv(getUniformLocation(varName), length, glm::value_ptr(values[0]));
};

void GLShaderProgram::setUniformImpl(const std::string& varName, const glm::dvec2* values, int length) {
   glUniform2dv(getUniformLocation(varName), length, glm::value_ptr(values[0]));
};
void GLShaderProgram::setUniformImpl(const std::string& varName, const glm::dvec3* values, int length) {
   glUniform3dv(getUniformLocation(varName), length, glm::value_ptr(values[0]));
};
void GLShaderProgram::setUniformImpl(const std::string& varName, const glm::dvec4* values, int length) {
   glUniform4dv(getUniformLocation(varName), length, glm::value_ptr(values[0]));
};


//glm matrix
void GLShaderProgram::setUniformImpl(const std::string& varName, const glm::mat2* values, int length) {
   glUniformMatrix2fv(getUniformLocation(varName), length, GL_FALSE, glm::value_ptr(values[0]));
};
void GLShaderProgram::setUniformImpl(const std::string& varName, const glm::mat3* values, int length) {
   glUniformMatrix3fv(getUniformLocation(varName), length, GL_FALSE, glm::value_ptr(values[0]));
};
void GLShaderProgram::setUniformImpl(const std::string& varName, const glm::mat4* values, int length) {
   glUniformMatrix4fv(getUniformLocation(varName), length, GL_FALSE, glm::value_ptr(values[0]));
};

void GLShaderProgram::setUniformImpl(const std::string& varName, const glm::dmat2* values, int length) {
   glUniformMatrix2dv(getUniformLocation(varName), length, GL_FALSE, glm::value_ptr(values[0]));
};
void GLShaderProgram::setUniformImpl(const std::string& varName, const glm::dmat3* values, int length) {
   glUniformMatrix3dv(getUniformLocation(varName), length, GL_FALSE, glm::value_ptr(values[0]));
};
void GLShaderProgram::setUniformImpl(const std::string& varName, const glm::dmat4* values, int length) {
   glUniformMatrix4dv(getUniformLocation(varName), length, GL_FALSE, glm::value_ptr(values[0]));
};

void GLShaderProgram::setUniformImpl(const std::string& varName, const ImageI* values, int length) {
   std::vector<int> channelIDs;
   channelIDs.reserve(length);
   for(int i = 0; i < length; i++) {
      const GLImage* image = dynamic_cast<const GLImage*>(&values[i]);
      channelIDs.push_back(image->textureChannel);
   } 
   glUniform1iv(getUniformLocation(varName), channelIDs.size(), channelIDs.data());
};


void GLShaderProgram::useProgram() {
   glUseProgram(shaderProgram);
}

GLShaderProgram::~GLShaderProgram() {
   glDeleteProgram(shaderProgram);
}


//Buffer:
GLBuffer::GLBuffer(GraphicsApiI::BufferType bufferType) {
   setType(bufferType);
   glGenBuffers(1, &buffer);
}

void GLBuffer::setType(GraphicsApiI::BufferType bufferType) {
   type = bufferType;
}

void GLBuffer::bind() {
   glBindBuffer(GLApi::getGlBufferType(type), buffer);
}

void GLBuffer::unbind() {
   glBindBuffer(GLApi::getGlBufferType(type), 0);
}

void* GLBuffer::getData(GraphicsApiI::BufferAccessType bufferAccessType) {
   return nullptr;
}

void GLBuffer::setData(const void* data, int size, GraphicsApiI::BufferUsageType bufferUsageType) {
   bind();
   glBufferData(GLApi::getGlBufferType(type), size, data, GLApi::getGlBufferUsageType(bufferUsageType));
}

GLBuffer::~GLBuffer() {
   unbind();
   glDeleteBuffers(1, &buffer);
}

//VertexLayout:
void GLVAL::addBuffer(BufferI* buffer) {
   buffers.push_back(buffer);
}

void GLVAL::addAttribLayout(GraphicsApiI::VertexLayoutAttrib rule) {
   rules.push_back(rule);
}

void GLVAL::bind() {
   std::vector<int> boundBufferIDs;
   for (auto& rule : rules) {
      if(rule.bufferID >= buffers.size()) return;
      BufferI* buffer = buffers[rule.bufferID];
      boundBufferIDs.push_back(rule.bufferID);
      buffer->bind();
      glVertexAttribPointer(rule.attribID, rule.dimmension, GLApi::getGlDataType(rule.attribType), GL_FALSE, rule.bufferElementSize, reinterpret_cast<void*>(static_cast<uintptr_t>(rule.offset)));
      glEnableVertexAttribArray(rule.attribID);
   }
   for(int i = 0; i < buffers.size(); i++) {
      if(std::find(boundBufferIDs.begin(), boundBufferIDs.end(), i) != boundBufferIDs.end()) continue;
      BufferI* buffer = buffers[i];
      buffer->bind();
   }
}

void GLVAL::unbind() {
   for (auto& rule : rules) {
      if(rule.bufferID >= buffers.size()) return;
      glDisableVertexAttribArray(rule.attribID);
   }
   for (auto buffer : buffers) {
      buffer->unbind();
   }
}

GLVAL::~GLVAL() {
   unbind();
}


//Image:
GLImage::GLImage(GraphicsApiI::TextureType texType) {
   setType(texType);
   glGenTextures(1, &texture);
}

void GLImage::setType(GraphicsApiI::TextureType texType) {
   type = texType;
}

bool GLImage::setChannel(unsigned int channelID) {
   if(channelID >= GLApi::getCombinedMaxTextureBindings()) return false;
   glActiveTexture(GL_TEXTURE0 + channelID);
   textureChannel = channelID;
   bind();
   return true;
}

bool GLImage::setData(const void* data, int size, GraphicsApiI::ApiDataType dataType, GraphicsApiI::TexFormatType texFormat, 
   unsigned int mipLevel, GraphicsApiI::TexFormatType externalTexFormat) {
   if(type != GraphicsApiI::TextureType::TEX_1D) return false;
   bind();
   glTexImage1D(
      GL_TEXTURE_1D, 
      mipLevel, 
      GLApi::getGlTexFormatType(texFormat), 
      size, 
      0, 
      externalTexFormat == GraphicsApiI::TexFormatType::UNDEFINED ? GLApi::getGlTexFormatType(texFormat) : GLApi::getGlTexFormatType(externalTexFormat), 
      GLApi::getGlDataType(dataType),
      data
   );
   return true;
}

bool GLImage::setData(const void* data, glm::ivec2 size, GraphicsApiI::ApiDataType dataType, GraphicsApiI::TexFormatType texFormat, 
   unsigned int mipLevel, GraphicsApiI::TexFormatType externalTexFormat) {
   if(type != GraphicsApiI::TextureType::TEX_2D) return false;
   bind();
   glTexImage2D(
      GL_TEXTURE_2D, 
      mipLevel, 
      GLApi::getGlTexFormatType(texFormat), 
      size.x,
      size.y,
      0, 
      externalTexFormat == GraphicsApiI::TexFormatType::UNDEFINED ? GLApi::getGlTexFormatType(texFormat) : GLApi::getGlTexFormatType(externalTexFormat), 
      GLApi::getGlDataType(dataType),
      data
   );
   return true;
}

bool GLImage::setData(const void* data, glm::ivec3 size, GraphicsApiI::ApiDataType dataType, GraphicsApiI::TexFormatType texFormat, 
   unsigned int mipLevel, GraphicsApiI::TexFormatType externalTexFormat) {
   if(type != GraphicsApiI::TextureType::TEX_3D) return false;
   bind();
   glTexImage3D(
      GL_TEXTURE_3D, 
      mipLevel, 
      GLApi::getGlTexFormatType(texFormat), 
      size.x, 
      size.y,
      size.z,
      0, 
      externalTexFormat == GraphicsApiI::TexFormatType::UNDEFINED ? GLApi::getGlTexFormatType(texFormat) : GLApi::getGlTexFormatType(externalTexFormat), 
      GLApi::getGlDataType(dataType),
      data
   );
   return true;
}

void GLImage::genMips() {
   bind();
   glGenerateMipmap(GLApi::getGlTextureType(type));
}

void GLImage::bind() {
   glBindTexture(GLApi::getGlTextureType(type), texture);
}

void GLImage::unbind() {
   glBindTexture(GLApi::getGlTextureType(type), 0);
}

GLImage::~GLImage() {
   unbind();
   glDeleteTextures(1, &texture);
}
