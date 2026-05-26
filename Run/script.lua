println("script started")

local genHeightMap = false;
if genHeightMap then
   local size = ivec2.new(100, 100)
   local heightMap = Array2D.new()

   heightMap.size = size;
   heightMap:resize()

   println("created heightMap")

   setNoise(NoiseType.CELLULAR)

   for x = 0, 99 do
      for y = 0, 99 do
         local h = noise(x, y) * 50;
         heightMap:set(x, y, h);
      end
   end

   local pos = vec3.new(0, 0, 0)
   local scale = vec3.new(1, 1, 1)
   local uvScale = vec2.new(1, 1)

   createHeightMap(heightMap, pos, scale, uvScale, true)

   println("Mesh created!")

else

   local size = ivec3.new(100, 100, 100)
   local voxels = Array3D.new()

   voxels.size = size;
   voxels:resize()

   println("created voxel map")

   setNoise(NoiseType.PERLIN)

   for x = 0, 99 do
      for y = 0, 99 do
         for z = 0, 99 do
            local w = noise(x*0.5, y*0.5, z*0.5)
            if y < 50 then
               w = 0
            end
            voxels:set(x, y, z, w)
         end
      end
   end

   local pos = vec3.new(0, 0, 0)
   local scale = vec3.new(1, 1, 1)
   local uvScale = vec2.new(1, 1)

   createSurface(voxels, pos, scale, uvScale, true, true, 0.0);

   println("Mesh created!")
end