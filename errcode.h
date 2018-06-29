#ifndef IMATCH_ERROR_NO_H_
#define IMATCH_ERROR_NO_H_
  
enum ih_Error
{
	OK = 0,
	SUCC = 0,
	ERROR_OK = 0, /// 成功

	////////////////////
	ERROR_FAIL,	/// 笼统性描述错误


	////////////////////
	ERROR_PARSE_PROTOBUF, /// 解析protobuf错误
	ERROR_SERIALIZE_PROTOBUF, /// 序列化protobuf错误
	ERROR_MEMORY_ALLOCATE, /// 内存分配错误
	ERROR_TIME_OUT, /// 超时
	ERROR_PRIVILEGE	, /// 没有权限
	ERROR_INVALID_APPCAT, /// 无效客户端类型
	ERROR_ARG, /// 参数错误
	ERROR_HAS_BEEN, /// 已经是
	ERROR_PARSE, /// 解析失败
	ERROR_SEND_DB, /// 发送dbsrv失败
	ERROR_DATA_SIZE_OVER,	// 数据长度过大
	ERROR_UNZIP_FAIL, // 解压失败
	
};



#endif // IMATCH_ERROR_NO_H_

