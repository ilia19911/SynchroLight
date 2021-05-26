DIR=${1}
SHA=$(git rev-parse HEAD)

printf "#ifndef _GIT_SHA_STRING_H_ \n #define _GIT_SHA_STRING_H_ \n char git_sha_string[] = \"%s\" ; \n #endif \n" $DIR$SHA > $DIR/git_sha_string.h

