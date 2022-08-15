DB=-g -g3 -ggdb
all: contain pack
contain:
	gcc generator.c -o generator -Wextra -Wall
	./generator
	rm generator
	gcc container.c -o container -Wextra -Wall -w $(DB)
pack:
	gcc packer.c -o packer -Wextra -Wall $(DB)
re:	fclean all
clean:
fclean:	clean
	rm -f packer container
.PHONY: contain pack
