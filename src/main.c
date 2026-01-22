#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum TokenType{
	LESS_THAN_SIGN,
	GREATER_THAN_SIGN,
	PLUS,
	MINUS,
	PERIOD,
	COMMA,
	LEFT_SQUARE_BRACKET,
	RIGHT_SQUARE_BRACKET,


	//NEW_LINE,
	//BLANK,
	//END
};

char* load_file(const char* filepath, size_t* out_size)
{
    FILE* f = fopen(filepath, "rb");
    if (!f) {
        perror("Failed to open file");
        return NULL;
    }

    if (fseek(f, 0, SEEK_END) != 0) {
        perror("fseek failed");
        fclose(f);
        return NULL;
    }

    long size = ftell(f);
    if (size < 0) {
        perror("ftell failed");
        fclose(f);
        return NULL;
    }

    if (fseek(f, 0, SEEK_SET) != 0) {
        perror("fseek failed");
        fclose(f);
        return NULL;
    }

    char* buffer = malloc((size_t)size + 1);
    if (!buffer) {
        perror("malloc failed");
        fclose(f);
        return NULL;
    }

    size_t read_bytes = fread(buffer, 1, (size_t)size, f);
    if (read_bytes != (size_t)size) {
        fprintf(stderr, "Failed to read entire file\n");
        free(buffer);
        fclose(f);
        return NULL;
    }

    buffer[size] = '\0';

    fclose(f);

    if (out_size) {
        *out_size = (size_t)size;
    }

    return buffer;
}

void append_token(enum TokenType t, int* array, int* n) {
    array[*n] = (int)t;
    (*n)++;
}

int* tokenize(const char* text, const size_t size, size_t* out_size){
    int* tokens = malloc(size * sizeof(int));
    if (!tokens) return NULL;
	//memset(tokens, -1, size * sizeof(int));

	int n = 0;

    for (size_t i = 0; text[i] != '\0'; i++) {
		switch(text[i]){
			case '<':
				append_token(LESS_THAN_SIGN, tokens, &n);
				break;
			case '>':
				append_token(GREATER_THAN_SIGN, tokens, &n);
				break;
			case '+':
				append_token(PLUS, tokens, &n);
				break;
			case '-':
				append_token(MINUS, tokens, &n);
				break;
			case '.':
				append_token(PERIOD, tokens, &n);
				break;
			case ',':
				append_token(COMMA, tokens, &n);
				break;	
			case '[':
				append_token(LEFT_SQUARE_BRACKET, tokens, &n);
				break;		
			case ']':
				append_token(RIGHT_SQUARE_BRACKET, tokens, &n);
				break;
			default:
				break;			
		}
    }

    int* t = realloc(tokens, n * sizeof(int));
    if (!t) {
        t = tokens;
    }

    *out_size = n;
    return t;
}

void interpret(int* tokens, size_t size){
	unsigned short int data[1024]; // tape
	memset(data, 0, 1024 * sizeof(unsigned short int));

	unsigned short int pointer_index = 0; // head


	for (size_t instruction_pointer = 0; instruction_pointer < size; instruction_pointer++){
		switch(tokens[instruction_pointer]){
			case LESS_THAN_SIGN:
				if (pointer_index > 0){
					pointer_index--;
				}
				break;
			case GREATER_THAN_SIGN:
				pointer_index++;
				break;
			case PLUS:
				data[pointer_index] = (data[pointer_index] + 1) % 256;
				break;
			case MINUS:
				data[pointer_index] = (data[pointer_index] + 255) % 256;
				break;
			case PERIOD:
				putchar(data[pointer_index]);
				printf("[%d]", data[pointer_index]);
				break;
			case COMMA:
				int c = getchar();
				if (c != EOF) data[pointer_index] = (unsigned char)c;
				else data[pointer_index] = 0;

				int dummy;
				while ((dummy = getchar()) != '\n' && c != EOF) { } // consuming and discarding
				break;
			case LEFT_SQUARE_BRACKET:
				if (data[pointer_index] == 0) {
					int loop = 1;
					while (loop > 0) {
						instruction_pointer++;
						if (tokens[instruction_pointer] == LEFT_SQUARE_BRACKET) loop++;
						else if (tokens[instruction_pointer] == RIGHT_SQUARE_BRACKET) loop--;
					}
				}
				break;

			case RIGHT_SQUARE_BRACKET:
				if (data[pointer_index] != 0) {
					int loop = 1;
					while (loop > 0) {
						instruction_pointer--;
						if (tokens[instruction_pointer] == LEFT_SQUARE_BRACKET) loop--;
						else if (tokens[instruction_pointer] == RIGHT_SQUARE_BRACKET) loop++;
					}
				}
				break;
			default:
				break;
		}
	}

}

int main(void) {
    size_t size = 0;
    char* text_buffer = load_file("brainfuck.txt", &size);
    if (text_buffer == NULL) {
        return 1;
    }

	size_t tokens_len = 0;
	int* tokens = tokenize(text_buffer, size, &tokens_len);
	
	/*
	for (size_t i = 0; i < tokens_len; i++){
		printf("%d ", tokens[i]);
	}
	printf("\n\n");*/
	//printf("%lld", sizeof(size_t));

	interpret(tokens, tokens_len);

    free(text_buffer);
	free(tokens);
    return 0;
}
