#ifndef _SOS_YAML_H
#define _SOS_YAML_H

enum keyword_e {
	FALSE_KW = 0,
	TRUE_KW = 1,
	ATTRIBUTE_KW = 100,
	INDEXED_KW,
	SCHEMA_KW,
	NAME_KW,
	TYPE_KW,
	COUNT_KW,
	TIME_FUNC_KW,
	RANDOM_FUNC_KW,
};

struct keyword {
	char *str;
	enum keyword_e id;
};

struct keyword keyword_table[] = {
	{ "ATTRIBUTE", ATTRIBUTE_KW },
	{ "BYTE_ARRAY", SOS_TYPE_BYTE_ARRAY },
	{ "DOUBLE", SOS_TYPE_DOUBLE },
	{ "DOUBLE_ARRAY", SOS_TYPE_DOUBLE_ARRAY },
	{ "FALSE", FALSE_KW },
	{ "FLOAT", SOS_TYPE_FLOAT },
	{ "FLOAT_ARRAY", SOS_TYPE_FLOAT_ARRAY },
	{ "INDEXED", INDEXED_KW },
	{ "INT32", SOS_TYPE_INT32 },
	{ "INT32_ARRAY", SOS_TYPE_INT32_ARRAY },
	{ "INT64", SOS_TYPE_INT64 },
	{ "INT64_ARRAY", SOS_TYPE_INT64_ARRAY },
	{ "LONG_DOUBLE", SOS_TYPE_LONG_DOUBLE },
	{ "LONG_DOUBLE_ARRAY", SOS_TYPE_LONG_DOUBLE_ARRAY },
	{ "NAME", NAME_KW },
	{ "OBJ", SOS_TYPE_OBJ },
	{ "OBJ_ARRAY", SOS_TYPE_OBJ_ARRAY },
	{ "RANDOM()", RANDOM_FUNC_KW },
	{ "SCHEMA", SCHEMA_KW },
	{ "TIME()", TIME_FUNC_KW },
	{ "TIMESTAMP", SOS_TYPE_TIMESTAMP },
	{ "TRUE", TRUE_KW },
	{ "TYPE", TYPE_KW },
	{ "UINT32", SOS_TYPE_UINT32 },
	{ "UINT32_ARRAY", SOS_TYPE_UINT32_ARRAY },
	{ "UINT64", SOS_TYPE_UINT64 },
	{ "UINT64_ARRAY", SOS_TYPE_UINT64_ARRAY },
};

int compare_keywords(const void *a, const void *b)
{
	struct keyword *kw_a = (struct keyword *)a;
	struct keyword *kw_b = (struct keyword *)b;
	return strcasecmp(kw_a->str, kw_b->str);
}

#endif