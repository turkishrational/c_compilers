#define FILE int  // FILE artýk bir pointer deðil, doðrudan int

// Fonksiyon imzalarýný buna göre güncelleyin
FILE fopen(const char* path, const char* mode);
int fgetc(FILE stream);