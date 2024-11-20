#include <aoc.h>
#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE 1000

typedef enum {
  FILE_STREAM,
  BUFFER_STREAM,
} stream_type_e;

typedef struct {
  stream_type_e type;
  size_t len;
  size_t position;
  void *object;
} stream_t;

stream_t *stream_buffer(char *buffer, size_t len) {
  stream_t *stream = (stream_t *)malloc(sizeof(stream_t));
  if (stream != NULL) {
    stream->object = malloc(sizeof(char) * len);
    if (stream->object == NULL) {
      free(stream);
      return NULL;
    }
    strncpy(stream->object, buffer, strnlen(buffer, len));

    stream->len = len;
    stream->position = 0;
    stream->type = BUFFER_STREAM;
  }

  return stream;
}

int stream_buffer_getchar(stream_t *buffer_stream) {
  assert(buffer_stream != NULL);
  assert(buffer_stream->type == BUFFER_STREAM);
  int c;

  if (buffer_stream->position >= buffer_stream->len) {
    return EOF;
  } else {
    c = ((char *)buffer_stream->object)[buffer_stream->position];
    buffer_stream->position += 1;
    return c;
  }
}

int stream_file_getchar(stream_t *file_stream) {
  assert(file_stream != NULL);
  assert(file_stream->type == FILE_STREAM);

  return EOF;
}

int stream_getchar(stream_t *stream) {
  assert(stream != NULL);
  switch (stream->type) {
  case BUFFER_STREAM:
    return stream_buffer_getchar(stream);
  case FILE_STREAM:
    return stream_file_getchar(stream);
  default:
    return EOF;
  }
}

int stream_buffer_peekchar(stream_t *stream) {
  assert(stream != NULL);
  assert(stream->type == BUFFER_STREAM);

  if (stream->position >= stream->len - 1) {
    return EOF;
  } else {
    return ((char *)stream->object)[stream->position];
  }
}

int stream_file_peekchar(stream_t *stream) {
  assert(stream != NULL);
  assert(stream->type == FILE_STREAM);

  return EOF;
}

int stream_peekchar(stream_t *stream) {
  assert(stream != NULL);
  switch (stream->type) {
  case BUFFER_STREAM:
    return stream_buffer_peekchar(stream);
  case FILE_STREAM:
    return stream_file_peekchar(stream);
  default:
    return EOF;
  }
}

int stream_charat(stream_t *stream, size_t index) {
  assert(stream != NULL);

  switch (stream->type) {
  case BUFFER_STREAM:
    if (index >= stream->len || index < 0) {
      return EOF;
    }
    return ((char *)stream->object)[index];
  default:
    return EOF;
  }
}

size_t stream_length(stream_t *stream) {
  assert(stream != NULL);

  return stream->len;
}

void stream_close(stream_t *stream) {
  if (stream == NULL) {
    return;
  }

  switch (stream->type) {
  case BUFFER_STREAM:
    free(stream->object);
    break;
  case FILE_STREAM:
    fclose(stream->object);
    break;
  }

  free(stream);
}

unsigned long get_escaped_length(char *line, size_t len) {
  stream_t *stream = stream_buffer(line, len);
  int c;
  unsigned long mem_len = 0;

  while ((c = stream_getchar(stream)) != EOF) {
    if (c == ' ' || c == '\n' || c == '\t') {
      continue;
    }

    if (c == '\\') {
      c = stream_peekchar(stream);

      if (c == '\\' || c == '"') {
        stream_getchar(stream);
      } else if (c == 'x') {
        stream_getchar(stream);
        c = stream_peekchar(stream);
        if (isdigit(c)) {
          stream_getchar(stream);
          c = stream_peekchar(stream);
          if (isdigit(c)) {
            stream_getchar(stream);
          } else {
            mem_len += 2;
          }
        } else {
          mem_len += 1;
        }
      }
    }
    mem_len += 1;
  }

  if ((c = stream_charat(stream, 0)) == '"') {
    mem_len -= 1;
  }

  if ((c = stream_charat(stream, stream_length(stream) - 1)) == '"') {
    mem_len -= 1;
  }

  stream_close(stream);
  return mem_len;
}

int main(void) {
  size_t slen;
  size_t mlen;
  char line[MAX_LINE] = {0};
  unsigned long code_length;
  unsigned long memory_length;

  code_length = memory_length = 0;

  while ((slen = aoc_get_line(line, MAX_LINE)) != 0) {
    mlen = get_escaped_length(line, slen);

    code_length += slen;
    memory_length += mlen;

    printf("%10lu | %10lu | %s\n", slen, mlen, line);
  }

  printf("code: %lu | memory: %lu | Result: %lu\n", code_length, memory_length,
         code_length - memory_length);

  return 0;
}
