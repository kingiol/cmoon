#ifndef __OINBOX_H__
#define __OINBOX_H__
#include "mheads.h"

__BEGIN_DECLS

/*
 * Description: 发送多个邮件（内部使用，非接口）
 * Input: datanode* - 数据hdf，递归该node的所有子node, 所有有 mid key 的将设为收件人地址
 *                    同时支持以下 template 子 node (详见mtemplate.h):
 *                     PRE_DATASET  :data used by template file on cs_rend()
 *                     PRE_VALUESET :copy to out direct
 *                     PRE_VALUEREP :replace valueset after valueset copied
 *        inboxtype* - 邮件类型字符串，支持所有tpl/config/inbox/xxx.hdf 下的子node key
 * Returns: None
 */
NEOERR* inbox_multi_add(HDF *datanode, HASH *evth, char *inboxtype);

__END_DECLS
#endif /* __OINBOX_H__ */
