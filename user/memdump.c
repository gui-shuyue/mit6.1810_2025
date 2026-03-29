#include "kernel/types.h"
#include "user/user.h"
#include "kernel/fcntl.h"

void memdump(char *fmt, char *data);

int
main(int argc, char *argv[])
{
  if(argc == 1){
    printf("Example 1:\n");
    int a[2] = { 61810, 2025 };
    memdump("ii", (char*) a);
    
    printf("Example 2:\n");
    memdump("S", "a string");
    
    printf("Example 3:\n");
    char *s = "another";
    memdump("s", (char *) &s);

    struct sss {
      char *ptr;
      int num1;
      short num2;
      char byte;
      char bytes[8];
    } example;
    
    example.ptr = "hello";
    example.num1 = 1819438967;
    example.num2 = 100;
    example.byte = 'z';
    strcpy(example.bytes, "xyzzy");
    
    printf("Example 4:\n");
    memdump("pihcS", (char*) &example);
    
    printf("Example 5:\n");
    memdump("sccccc", (char*) &example);
  } else if(argc == 2){
    // format in argv[1], up to 512 bytes of data from standard input.
    char data[512];
    int n = 0;
    memset(data, '\0', sizeof(data));
    while(n < sizeof(data)){
      int nn = read(0, data + n, sizeof(data) - n);
      if(nn <= 0)
        break;
      n += nn;
    }
    memdump(argv[1], data);
  } else {
    printf("Usage: memdump [format]\n");
    exit(1);
  }
  exit(0);
}

void
memdump(char *fmt, char *data)
{
  char *p = data;

  for (int i = 0; fmt[i] != '\0'; i++) {
    switch (fmt[i]) {
      case 'i':
        // 4字节 32-bit 整数，打印为十进制
        printf("%d\n", *(int*)p);
        p += sizeof(int);
        break;
      case 'p':
        // 8字节 64-bit 指针，打印为十六进制
        // C 的 %p 需要 void* 参数
        uint64 ptr_val = *(uint64*)p;
        printf("%p\n", (void*)ptr_val);
        p += sizeof(uint64);
        break;
      case 'h':
        printf("%d\n", *(short*)p);
        p += sizeof(short);
        break;
      case 'c':
        printf("%c\n", *(char*)p);
        p += sizeof(char);
        break;
      case 's':
        // 内存里存的是一个 8 字节的地址，该地址指向字符串
        char *sptr = *(char **)p;
        printf("%s\n", sptr);
        p += 8;
        break;
      case 'S':
        // 内存里直接就是字符串数据
        printf("%s\n", (char *)p);
        // 题目要求 S 处理剩余数据，或者移动到 \0 之后
        p += strlen((char *)p) + 1;
        break;
      default:
        printf("Unknown format character: %c\n", fmt[i]);
        return;
    }
  }

}
