---@meta

-- ENUMS

---@enum NoiseType
NoiseType = {
   PERLIN = 0,
   CELLULAR = 1,
   SIMPLEX = 2
}

-- GLM TYPES

---@class ivec2
---@field x number
---@field y number
ivec2 = {}

---@param x number
---@param y number
---@return ivec2
function ivec2.new(x, y) end


---@class ivec3
---@field x number
---@field y number
---@field z number
ivec3 = {}

---@param x number
---@param y number
---@param z number
---@return ivec3
function ivec3.new(x, y, z) end


---@class vec2
---@field x number
---@field y number
vec2 = {}

---@param x number
---@param y number
---@return vec2
function vec2.new(x, y) end


---@class vec3
---@field x number
---@field y number
---@field z number
vec3 = {}

---@param x number
---@param y number
---@param z number
---@return vec3
function vec3.new(x, y, z) end


-- ARRAYS

---@class Array2D
---@field size ivec2
Array2D = {}

---@return Array2D
function Array2D.new() end

function Array2D:resize() end

---@param x number
---@param y number
---@return number
function Array2D:get(x, y) end

---@param x number
---@param y number
---@param v number
function Array2D:set(x, y, v) end


---@class Array3D
---@field size ivec3
Array3D = {}

---@return Array3D
function Array3D.new() end

function Array3D:resize() end

---@param x number
---@param y number
---@param z number
---@return number
function Array3D:get(x, y, z) end

---@param x number
---@param y number
---@param z number
---@param v number
function Array3D:set(x, y, z, v) end


-- ENGINE FUNCTIONS

---@param type NoiseType
function setNoise(type) end

---@param x number
---@param y number
---@param z number
---@return number
function noise(x, y, z) end

---@param x number
---@param y number
---@return number
function noise(x, y) end


---@param array Array2D
---@param position vec3
---@param scale vec3
---@param uvScale vec2
---@param smooth boolean
function createHeightMap(array, position, scale, uvScale, smooth) end

---@param array Array3D
---@param position vec3
---@param scale vec3
---@param uvScale vec2
---@param interpolation boolean
---@param smooth boolean
---@param iso number
function createSurface(array, position, scale, uvScale, interpolation, smooth, iso) end

function print(...) end
function println(...) end