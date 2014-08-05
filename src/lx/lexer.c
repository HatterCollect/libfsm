/* Generated by lx */

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#include <unistd.h>
#include <fcntl.h>

#include LX_HEADER

static enum lx_token z1(struct lx *lx);
static enum lx_token z2(struct lx *lx);
static enum lx_token z3(struct lx *lx);
static enum lx_token z4(struct lx *lx);
static enum lx_token z5(struct lx *lx);

static int
lx_getc(struct lx *lx)
{
	int c;

	assert(lx != NULL);
	assert(lx->lgetc != NULL);

	if (lx->c != EOF) {
		c = lx->c, lx->c = EOF;
	} else {
		c = lx->lgetc(lx);
		if (c == EOF) {
			return EOF;
		}
	}

	if (c == '\n') {
		lx->line++;
	}

	lx->byte++;

	return c;
}

static void
lx_ungetc(struct lx *lx, int c)
{
	assert(lx != NULL);
	assert(lx->c == EOF);

	lx->c = c;

	if (lx->pop != NULL) {
		lx->pop(lx);
	}

	if (c == '\n') {
		lx->line--;
	}

	lx->byte--;
}

int
lx_fgetc(struct lx *lx)
{
	assert(lx != NULL);
	assert(lx->opaque != NULL);

	return fgetc(lx->opaque);
}

int
lx_sgetc(struct lx *lx)
{
	char *s;

	assert(lx != NULL);
	assert(lx->opaque != NULL);

	s = lx->opaque;
	if (*s == '\0') {
		return EOF;
	}

	return lx->opaque = s + 1, *s;
}

int
lx_agetc(struct lx *lx)
{
	struct lx_arr *a;

	assert(lx != NULL);
	assert(lx->opaque != NULL);

	a = lx->opaque;

	assert(a != NULL);
	assert(a->p != NULL);

	if (a->len == 0) {
		return EOF;
	}

	return a->len--, *a->p++;
}

int
lx_dgetc(struct lx *lx)
{
	struct lx_fd *d;

	assert(lx != NULL);
	assert(lx->opaque != NULL);

	d = lx->opaque;
	assert(d->fd != -1);
	assert(d->p != NULL);

	if (d->len == 0) {
		ssize_t r;

		assert((fcntl(d->fd, F_GETFL) & O_NONBLOCK) == 0);

		d->p = (char *) d + sizeof *d;

		r = read(d->fd, d->p, d->bufsz);
		if (r == -1) {
			assert(errno != EAGAIN);
			return EOF;
		}

		if (r == 0) {
			return EOF;
		}

		d->len = r;
	}

	return d->len--, *d->p++;
}

int
lx_dynpush(struct lx *lx, char c)
{
	struct lx_dynbuf *t;

	assert(lx != NULL);
	assert(c != EOF);

	t = lx->buf;

	assert(t != NULL);

	if (t->p == t->a + t->len) {
		size_t len;
		char *tmp;

		if (t->len == 0) {
			assert(LX_DYN_LOW > 0);
			len = LX_DYN_LOW;
		} else {
			len = t->len * LX_DYN_FACTOR;
			if (len < t->len) {
				errno = ERANGE;
				return -1;
			}
		}

		tmp = realloc(t->a, len);
		if (tmp == NULL) {
			return -1;
		}

		t->p   = tmp + (t->p - t->a);
		t->a   = tmp;
		t->len = len;
	}

	assert(t->p != NULL);
	assert(t->a != NULL);

	*t->p++ = c;

	return 0;
}

void
lx_dynpop(struct lx *lx)
{
	struct lx_dynbuf *t;

	assert(lx != NULL);

	t = lx->buf;

	assert(t != NULL);
	assert(t->a != NULL);
	assert(t->p >= t->a);

	if (t->p == t->a) {
		return;
	}

	t->p--;
}

int
lx_dynclear(struct lx *lx)
{
	struct lx_dynbuf *t;

	assert(lx != NULL);

	t = lx->buf;

	assert(t != NULL);

	if (t->len > LX_DYN_HIGH) {
		size_t len;
		char *tmp;

		len = t->len / LX_DYN_FACTOR;

		tmp = realloc(t->a, len);
		if (tmp == NULL) {
			return -1;
		}

		t->a   = tmp;
		t->len = len;
	}

	t->p = t->a;

	return 0;
}

void
lx_dynfree(struct lx *lx)
{
	struct lx_dynbuf *t;

	assert(lx != NULL);

	t = lx->buf;

	assert(t != NULL);

	free(t->a);
}
int
lx_fixedpush(struct lx *lx, char c)
{
	struct lx_fixedbuf *t;

	assert(lx != NULL);
	assert(c != EOF);

	t = lx->buf;

	assert(t != NULL);
	assert(t->p != NULL);
	assert(t->a != NULL);

	if (t->p == t->a + t->len) {
		errno = ENOMEM;
		return -1;
	}

	*t->p++ = c;

	return 0;
}

void
lx_fixedpop(struct lx *lx)
{
	struct lx_fixedbuf *t;

	assert(lx != NULL);

	t = lx->buf;

	assert(t != NULL);
	assert(t->a != NULL);
	assert(t->p >= t->a);

	if (t->p == t->a) {
		return;
	}

	t->p--;
}

int
lx_fixedclear(struct lx *lx)
{
	struct lx_fixedbuf *t;

	assert(lx != NULL);

	t = lx->buf;

	assert(t != NULL);
	assert(t->p != NULL);
	assert(t->a != NULL);

	t->p = t->a;

	return 0;
}

static enum lx_token
z1(struct lx *lx)
{
	int c;

	enum {
		S1, S2, S3, S4
	} state;

	assert(lx != NULL);

	if (lx->clear != NULL) {
		lx->clear(lx);
	}

	state = S4;

	while (c = lx_getc(lx), c != EOF) {
		switch (state) {
			break;

		default:
			if (lx->push != NULL) {
				if (-1 == lx->push(lx, c)) {
					return TOK_ERROR;
				}
			}
			break;

		}

		switch (state) {
		case S1:
			switch (c) {
			default:  lx_ungetc(lx, c); return lx->z = z5, TOK_RE;
			}

		case S2:
			switch (c) {
			case 'i': state = S1;      continue;
			default:  lx_ungetc(lx, c); return lx->z = z5, TOK_RE;
			}

		case S3:
			switch (c) {
			default:  lx_ungetc(lx, c); return TOK_CHAR;
			}

		case S4:
			switch (c) {
			case '/': state = S2;      continue;
			default:  state = S3;     continue;
			}
		}
	}

	lx->lgetc = NULL;

	switch (state) {
	case S1: return TOK_RE;
	case S2: return TOK_RE;
	case S3: return TOK_CHAR;
	default: errno = EINVAL; return TOK_ERROR;
	}
}

static enum lx_token
z2(struct lx *lx)
{
	int c;

	enum {
		S1, S2, S3, S4, S5
	} state;

	assert(lx != NULL);

	if (lx->clear != NULL) {
		lx->clear(lx);
	}

	state = S5;

	while (c = lx_getc(lx), c != EOF) {
		switch (state) {
			break;

		default:
			if (lx->push != NULL) {
				if (-1 == lx->push(lx, c)) {
					return TOK_ERROR;
				}
			}
			break;

		}

		switch (state) {
		case S1:
			switch (c) {
			default:  lx_ungetc(lx, c); return TOK_ESC;
			}

		case S2:
			switch (c) {
			default:  lx_ungetc(lx, c); return lx->z = z5, TOK_STR;
			}

		case S3:
			switch (c) {
			case '\\':
			case 'n':
			case 'r':
			case 't': state = S1;      continue;
			default:  lx->lgetc = NULL; return TOK_UNKNOWN;
			}

		case S4:
			switch (c) {
			default:  lx_ungetc(lx, c); return TOK_CHAR;
			}

		case S5:
			switch (c) {
			case '\"': state = S2;      continue;
			case '\\': state = S3;      continue;
			default:  state = S4;     continue;
			}
		}
	}

	lx->lgetc = NULL;

	switch (state) {
	case S1: return TOK_ESC;
	case S2: return TOK_STR;
	case S4: return TOK_CHAR;
	default: errno = EINVAL; return TOK_ERROR;
	}
}

static enum lx_token
z3(struct lx *lx)
{
	int c;

	enum {
		S1, S2, S3
	} state;

	assert(lx != NULL);

	if (lx->clear != NULL) {
		lx->clear(lx);
	}

	state = S3;

	while (c = lx_getc(lx), c != EOF) {
		switch (state) {
			break;

		default:
			if (lx->push != NULL) {
				if (-1 == lx->push(lx, c)) {
					return TOK_ERROR;
				}
			}
			break;

		}

		switch (state) {
		case S1:
			switch (c) {
			default:  lx_ungetc(lx, c); return lx->z = z5, TOK_STR;
			}

		case S2:
			switch (c) {
			default:  lx_ungetc(lx, c); return TOK_CHAR;
			}

		case S3:
			switch (c) {
			case '\'': state = S1;      continue;
			default:  state = S2;     continue;
			}
		}
	}

	lx->lgetc = NULL;

	switch (state) {
	case S1: return TOK_STR;
	case S2: return TOK_CHAR;
	default: errno = EINVAL; return TOK_ERROR;
	}
}

static enum lx_token
z4(struct lx *lx)
{
	int c;

	enum {
		S1, S2, S3
	} state;

	assert(lx != NULL);

	if (lx->clear != NULL) {
		lx->clear(lx);
	}

	state = S3;

	while (c = lx_getc(lx), c != EOF) {
		switch (state) {
		case S1:
		case S2:
		case S3:
			break;

		default:
			if (lx->push != NULL) {
				if (-1 == lx->push(lx, c)) {
					return TOK_ERROR;
				}
			}
			break;

		}

		switch (state) {
		case S1:
			switch (c) {
			default:  lx_ungetc(lx, c); return lx->z = z5, lx->z(lx);
			}

		case S2:
			switch (c) {
			default:  lx_ungetc(lx, c); return lx->z(lx);
			}

		case S3:
			switch (c) {
			case '\n': state = S1;      continue;
			default:  state = S2;     continue;
			}
		}
	}

	lx->lgetc = NULL;

	switch (state) {
	case S1: return TOK_EOF;
	case S2: return TOK_EOF;
	default: errno = EINVAL; return TOK_ERROR;
	}
}

static enum lx_token
z5(struct lx *lx)
{
	int c;

	enum {
		S1, S2, S3, S4, S5, S6, S7, S8, S9, S10, 
		S11, S12, S13, S14, S15, S16, S17, S18, S19, S20, 
		S21, S22, S23, S24, S25
	} state;

	assert(lx != NULL);

	if (lx->clear != NULL) {
		lx->clear(lx);
	}

	state = S25;

	while (c = lx_getc(lx), c != EOF) {
		switch (state) {
		case S4:
		case S6:
		case S7:
		case S9:
		case S16:
			break;

		default:
			if (lx->push != NULL) {
				if (-1 == lx->push(lx, c)) {
					return TOK_ERROR;
				}
			}
			break;

		}

		switch (state) {
		case S1:
			switch (c) {
			default:  lx_ungetc(lx, c); return TOK_TO;
			}

		case S2:
			switch (c) {
			default:  lx_ungetc(lx, c); return TOK_MAP;
			}

		case S3:
			switch (c) {
			case '0':
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':
			case 'A':
			case 'B':
			case 'C':
			case 'D':
			case 'E':
			case 'F':
			case 'G':
			case 'H':
			case 'I':
			case 'J':
			case 'K':
			case 'L':
			case 'M':
			case 'N':
			case 'O':
			case 'P':
			case 'Q':
			case 'R':
			case 'S':
			case 'T':
			case 'U':
			case 'V':
			case 'W':
			case 'X':
			case 'Y':
			case 'Z':
			case '_':
			case 'a':
			case 'b':
			case 'c':
			case 'd':
			case 'e':
			case 'f':
			case 'g':
			case 'h':
			case 'i':
			case 'j':
			case 'k':
			case 'l':
			case 'm':
			case 'n':
			case 'o':
			case 'p':
			case 'q':
			case 'r':
			case 's':
			case 't':
			case 'u':
			case 'v':
			case 'w':
			case 'x':
			case 'y':
			case 'z':	          continue;
			default:  lx_ungetc(lx, c); return TOK_TOKEN;
			}

		case S4:
			switch (c) {
			case '\t':
			case '\n':
			case '\r':
			case ' ':	          continue;
			default:  lx_ungetc(lx, c); return lx->z(lx);
			}

		case S5:
			switch (c) {
			default:  lx_ungetc(lx, c); return TOK_BANG;
			}

		case S6:
			switch (c) {
			default:  lx_ungetc(lx, c); return lx->z = z2, lx->z(lx);
			}

		case S7:
			switch (c) {
			default:  lx_ungetc(lx, c); return lx->z = z4, lx->z(lx);
			}

		case S8:
			switch (c) {
			case 'A':
			case 'B':
			case 'C':
			case 'D':
			case 'E':
			case 'F':
			case 'G':
			case 'H':
			case 'I':
			case 'J':
			case 'K':
			case 'L':
			case 'M':
			case 'N':
			case 'O':
			case 'P':
			case 'Q':
			case 'R':
			case 'S':
			case 'T':
			case 'U':
			case 'V':
			case 'W':
			case 'X':
			case 'Y':
			case 'Z':
			case '_':
			case 'a':
			case 'b':
			case 'c':
			case 'd':
			case 'e':
			case 'f':
			case 'g':
			case 'h':
			case 'i':
			case 'j':
			case 'k':
			case 'l':
			case 'm':
			case 'n':
			case 'o':
			case 'p':
			case 'q':
			case 'r':
			case 's':
			case 't':
			case 'u':
			case 'v':
			case 'w':
			case 'x':
			case 'y':
			case 'z': state = S3;      continue;
			default:  lx->lgetc = NULL; return TOK_UNKNOWN;
			}

		case S9:
			switch (c) {
			default:  lx_ungetc(lx, c); return lx->z = z3, lx->z(lx);
			}

		case S10:
			switch (c) {
			default:  lx_ungetc(lx, c); return TOK_LPAREN;
			}

		case S11:
			switch (c) {
			default:  lx_ungetc(lx, c); return TOK_RPAREN;
			}

		case S12:
			switch (c) {
			default:  lx_ungetc(lx, c); return TOK_STAR;
			}

		case S13:
			switch (c) {
			default:  lx_ungetc(lx, c); return TOK_CROSS;
			}

		case S14:
			switch (c) {
			case '>': state = S2;      continue;
			default:  lx_ungetc(lx, c); return TOK_DASH;
			}

		case S15:
			switch (c) {
			case '.': state = S1;      continue;
			default:  lx_ungetc(lx, c); return TOK_DOT;
			}

		case S16:
			switch (c) {
			default:  lx_ungetc(lx, c); return lx->z = z1, lx->z(lx);
			}

		case S17:
			switch (c) {
			default:  lx_ungetc(lx, c); return TOK_SEMI;
			}

		case S18:
			switch (c) {
			default:  lx_ungetc(lx, c); return TOK_BIND;
			}

		case S19:
			switch (c) {
			default:  lx_ungetc(lx, c); return TOK_QMARK;
			}

		case S20:
			switch (c) {
			case '0':
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':
			case 'A':
			case 'B':
			case 'C':
			case 'D':
			case 'E':
			case 'F':
			case 'G':
			case 'H':
			case 'I':
			case 'J':
			case 'K':
			case 'L':
			case 'M':
			case 'N':
			case 'O':
			case 'P':
			case 'Q':
			case 'R':
			case 'S':
			case 'T':
			case 'U':
			case 'V':
			case 'W':
			case 'X':
			case 'Y':
			case 'Z':
			case '_':
			case 'a':
			case 'b':
			case 'c':
			case 'd':
			case 'e':
			case 'f':
			case 'g':
			case 'h':
			case 'i':
			case 'j':
			case 'k':
			case 'l':
			case 'm':
			case 'n':
			case 'o':
			case 'p':
			case 'q':
			case 'r':
			case 's':
			case 't':
			case 'u':
			case 'v':
			case 'w':
			case 'x':
			case 'y':
			case 'z':	          continue;
			default:  lx_ungetc(lx, c); return TOK_IDENT;
			}

		case S21:
			switch (c) {
			default:  lx_ungetc(lx, c); return TOK_OPEN;
			}

		case S22:
			switch (c) {
			default:  lx_ungetc(lx, c); return TOK_PIPE;
			}

		case S23:
			switch (c) {
			default:  lx_ungetc(lx, c); return TOK_CLOSE;
			}

		case S24:
			switch (c) {
			default:  lx_ungetc(lx, c); return TOK_TILDE;
			}

		case S25:
			switch (c) {
			case '\t':
			case '\n':
			case '\r':
			case ' ': state = S4;      continue;
			case '!': state = S5;      continue;
			case '\"': state = S6;      continue;
			case '#': state = S7;      continue;
			case '$': state = S8;      continue;
			case '\'': state = S9;      continue;
			case '(': state = S10;      continue;
			case ')': state = S11;      continue;
			case '*': state = S12;      continue;
			case '+': state = S13;      continue;
			case '-': state = S14;      continue;
			case '.': state = S15;      continue;
			case '/': state = S16;      continue;
			case ';': state = S17;      continue;
			case '=': state = S18;      continue;
			case '?': state = S19;      continue;
			case 'A':
			case 'B':
			case 'C':
			case 'D':
			case 'E':
			case 'F':
			case 'G':
			case 'H':
			case 'I':
			case 'J':
			case 'K':
			case 'L':
			case 'M':
			case 'N':
			case 'O':
			case 'P':
			case 'Q':
			case 'R':
			case 'S':
			case 'T':
			case 'U':
			case 'V':
			case 'W':
			case 'X':
			case 'Y':
			case 'Z':
			case '_':
			case 'a':
			case 'b':
			case 'c':
			case 'd':
			case 'e':
			case 'f':
			case 'g':
			case 'h':
			case 'i':
			case 'j':
			case 'k':
			case 'l':
			case 'm':
			case 'n':
			case 'o':
			case 'p':
			case 'q':
			case 'r':
			case 's':
			case 't':
			case 'u':
			case 'v':
			case 'w':
			case 'x':
			case 'y':
			case 'z': state = S20;      continue;
			case '{': state = S21;      continue;
			case '|': state = S22;      continue;
			case '}': state = S23;      continue;
			case '~': state = S24;      continue;
			default:  lx->lgetc = NULL; return TOK_UNKNOWN;
			}
		}
	}

	lx->lgetc = NULL;

	switch (state) {
	case S1: return TOK_TO;
	case S2: return TOK_MAP;
	case S3: return TOK_TOKEN;
	case S4: return TOK_EOF;
	case S5: return TOK_BANG;
	case S6: return TOK_EOF;
	case S7: return TOK_EOF;
	case S9: return TOK_EOF;
	case S10: return TOK_LPAREN;
	case S11: return TOK_RPAREN;
	case S12: return TOK_STAR;
	case S13: return TOK_CROSS;
	case S14: return TOK_DASH;
	case S15: return TOK_DOT;
	case S16: return TOK_EOF;
	case S17: return TOK_SEMI;
	case S18: return TOK_BIND;
	case S19: return TOK_QMARK;
	case S20: return TOK_IDENT;
	case S21: return TOK_OPEN;
	case S22: return TOK_PIPE;
	case S23: return TOK_CLOSE;
	case S24: return TOK_TILDE;
	default: errno = EINVAL; return TOK_ERROR;
	}
}

const char *
lx_name(enum lx_token t)
{
	switch (t) {
	case TOK_IDENT: return "IDENT";
	case TOK_TOKEN: return "TOKEN";
	case TOK_PIPE: return "PIPE";
	case TOK_DOT: return "DOT";
	case TOK_BANG: return "BANG";
	case TOK_DASH: return "DASH";
	case TOK_TILDE: return "TILDE";
	case TOK_QMARK: return "QMARK";
	case TOK_CROSS: return "CROSS";
	case TOK_STAR: return "STAR";
	case TOK_RPAREN: return "RPAREN";
	case TOK_LPAREN: return "LPAREN";
	case TOK_CLOSE: return "CLOSE";
	case TOK_OPEN: return "OPEN";
	case TOK_MAP: return "MAP";
	case TOK_TO: return "TO";
	case TOK_SEMI: return "SEMI";
	case TOK_BIND: return "BIND";
	case TOK_RE: return "RE";
	case TOK_ESC: return "ESC";
	case TOK_STR: return "STR";
	case TOK_CHAR: return "CHAR";
	case TOK_EOF:     return "EOF";
	case TOK_ERROR:   return "ERROR";
	case TOK_UNKNOWN: return "UNKNOWN";
	default: return "?";
	}
}

void
lx_init(struct lx *lx)
{
	const static struct lx lx_default;

	assert(lx != NULL);

	*lx = lx_default;

	lx->c = EOF;
	lx->z = NULL;

	lx->line = 0;
	lx->byte = 0;
}

enum lx_token
lx_next(struct lx *lx)
{
	enum lx_token t;

	assert(lx != NULL);

	if (lx->lgetc == NULL) {
		return TOK_EOF;
	}

	if (lx->z == NULL) {
		lx->z = z5;
	}

	t = lx->z(lx);
	if (lx->lgetc == NULL && lx->free != NULL) {
		lx->free(lx);
	}

	if (lx->push != NULL) {
		if (-1 == lx->push(lx, '\0')) {
			return TOK_ERROR;
		}
	}

	return t;
}

