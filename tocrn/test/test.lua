local ffi = require("ffi")

--[[
    压缩图片到crunch后的ETC2
        pSrcData jpep、png 的原始数据
        srcSize 输入数据的字节数
        pOutSize 编码后数据字节数
        函数返回 编码后的数据 编码失败返回 nullptr
    
    void* compress(const unsigned char *pSrcData,   unsigned int scrSize, unsigned int *pOutSize);

    释放压缩函数返回的内存
    void freeCompressData(void* pOutData);
]]

ffi.cdef[[
void* compress(const unsigned char *pSrcData,   unsigned int scrSize, unsigned int *pOutSize);
void freeCompressData(void* pOutData);
]]

tocrn = ffi.load('./libtocrn.so')

local file  = io.open("test.jpeg", "rb")
local data = file:read("*a")
local size = file:seek("end") 
file:close()

local pOutSize = ffi.new("int[1]")
local outData  = tocrn.compress(data, size, pOutSize)
print(pOutSize[0])
print(outData)

local fileOut = io.open("test.crn", "wb")
fileOut:write(ffi.string(outData, pOutSize[0]))
fileOut:close()

tocrn.freeCompressData(outData)
