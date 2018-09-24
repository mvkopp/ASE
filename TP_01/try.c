#include <assert.h>
#include "try.h"

int try
    (struct ctx_s *pctx, func_t *f, int arg)
    {
        // initialiser le champ magic
        pctx->magic = CTX_MAGIC;

        // Mémoriser %esp dans contexte
        asm("movl %%esp, %0"
            : "=r"(pctx->esp));

        // Mémoriser %ebp dans contexte
        asm("movl %%ebp, %0"
            : "=r"(pctx->ebp));

        // appeler la fonction
        return f(arg);
    }

int throw(struct ctx_s * pctx, int r)
{
    // pour retourner r, il faut l'enregistrer en static car quand on change de registre, on n'y a plus accès
    static int copy_r = 0;

    // Check si pctx->magic est bien égal à CTX_MAGIX
    assert(pctx->magic == CTX_MAGIC);
    pctx->magic = 0;

    copy_r = r;

    // Placer ebp et esp du contexte dans les registres
    asm("movl %0, %%esp"
        :
        : "r"(pctx->esp));

    asm("movl %0, %%ebp"
        :
        : "r"(pctx->ebp));

    // renvoyer l'entier en paramètre (changement de registre, donc on renvoie la copie statique)
    return copy_r;
}