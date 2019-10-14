#include<iostream>
#include<fstream>
#define chunkSize 512000


bool chunkify(const char *filename)
{
  void *buffer = malloc(chunkSize);
  FILE *fin;
  bool ok = true;

  if (buffer == NULL)
    return false;

  if ((fin = fopen(filename, "rb")) != NULL)
  {
    fseek(fin , 0 , SEEK_END);
    int size = ftell(fin);
    rewind(fin);
    send(client_fd , &size , sizeof(size) , 0);
    size_t got;
    unsigned int count = 0;

    while(size > 0 && (got = fread(buffer, 1, chunkSize, fin)) > 0)
    {
      FILE *fout;
      char outname[512];
      snprintf(outname, sizeof outname, "%s-%u", filename, count++);
      if ((fout = fopen(outname, "wb")) != NULL)
      {
        ok &= fwrite(buffer, got, 1, fout);
        fclose(fout);
      }

      
    }
    fclose(fin);
  }
  free(buffer);
  return ok;
}

int main()
{

  chunkify("temp.txt");

  return 0;
}