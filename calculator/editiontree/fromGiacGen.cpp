#include "EditionTree.hpp"

#include <iostream>
#include <giac/config.h>
#include <giac/gen.h>
// #include <giac/dispatch.h>
// #include <giac/usual.h>

#define EXT_GIAC_CONTEXT const giac::context * contextptr

void atInt(int a, EditionTree &shell, EXT_GIAC_CONTEXT);
void atLeaf(const giac::gen &e, EditionTree &shell, EXT_GIAC_CONTEXT);
void atPlus(const giac::symbolic &e, EditionTree &shell, EXT_GIAC_CONTEXT);
void atNeg(const giac::symbolic &e, EditionTree &shell, EXT_GIAC_CONTEXT);
void atProd(const giac::symbolic &e, EditionTree &shell, EXT_GIAC_CONTEXT);
void atInv(const giac::gen &e, EditionTree &shell, EXT_GIAC_CONTEXT);

void atSymbolic(const giac::symbolic &e, EditionTree &shell, EXT_GIAC_CONTEXT);
void atGen(const giac::gen &e, EditionTree &shell, EXT_GIAC_CONTEXT);

void atInt(int a, EditionTree &shell, EXT_GIAC_CONTEXT)
{
    char lastEdition = shell.getLastEdition();
    std::cout << "@atInt with last edition " << lastEdition << "\n";
    if (a < 0) {
        if (lastEdition == '+') {
            shell.editDelete();
            shell.editOperator('-', "-");
        } else if (lastEdition == '-') {
            shell.editDelete();
            shell.editOperator('+', "+");
        } else
            shell.editChar((giac::calc_mode(contextptr) == 38) ? '-' : '-');
        a = -a;
    }
    std::string number = std::to_string(a);
    shell.editStr(number);
    return;
}

void atLeaf(const giac::gen &e, EditionTree &shell, EXT_GIAC_CONTEXT)
{
    std::cout << "@atLeaf\n";
    std::string str = e.print();
    std::cout << "atLeaf : " << str << std::endl;
    shell.editStr(str);
}

void atPlus(const giac::symbolic &e, EditionTree &shell, EXT_GIAC_CONTEXT)
{
    std::cout << "@atPlus\n";

    if (giac::is_inf(e.feuille) && giac::abs_calc_mode(contextptr) == 38)
        return shell.editChar('?');
    if (e.feuille.type != giac::_VECT) {
        shell.editOperator('+', "+");
        return atGen(e.feuille, shell, contextptr);
    }

    giac::vecteur &v = *e.feuille._VECTptr;
    if (v.empty())
        return;

    int l = int(v.size());
    if (l == 1)
        shell.editOperator('+', "+");

    atGen(v.front(), shell, contextptr);
    for (int i = 1; i < l ; i++) {
        shell.editOperator('+', "+");
        atGen(v[i], shell, contextptr);
    }
    return;
}

void atNeg(const giac::gen &e, EditionTree &shell, EXT_GIAC_CONTEXT)
{
    std::cout << "@atNeg\n";
    char lastEdition = shell.getLastEdition();
    if (lastEdition == '+') {
        shell.editDelete();
        shell.editOperator('-', "-");
    } else if (lastEdition == '-') {
        shell.editDelete();
        shell.editOperator('-', QString("－"));
    }

    if (e.type != giac::_CPLX)
        if (e.type != giac::_SYMB
                || e._SYMBptr->sommet == giac::at_inv
                || e._SYMBptr->sommet == giac::at_prod
                || !giac::need_parenthesis(e)) {
            atGen(e, shell, contextptr);
            return;
        }

    shell.editParen(LPAREN);
    atGen(e, shell, contextptr);
    shell.editParen(RPAREN);
    return;
}

void atProd(const giac::symbolic &e, EditionTree &shell, EXT_GIAC_CONTEXT)
{
    std::cout << "@atProd\n";
    giac::gen n0, d0;
    if (giac::print_rewrite_prod_inv && giac::rewrite_prod_inv(e.feuille, n0, d0)) {
        shell.editFrac();
        atGen(n0, shell, contextptr);
        shell.editMoveDown();
        atGen(d0, shell, contextptr);
        shell.editMoveRight();
        return;
    }

    if (e.feuille.type != giac::_VECT)
        return atGen(e.feuille, shell, contextptr);
    giac::vecteur &v = *e.feuille._VECTptr;
    int l = int(v.size());

    for (int i = 0; i < l; i++) {
        giac::gen f(v[i]);
        if (f.type != giac::_SYMB) {
            if (i)
                shell.editOperator('+', QString("×"));
            if ((f.type == giac::_CPLX && giac::need_parenthesis(f))
                    || f.type == giac::_MOD) {
                shell.editParen(LPAREN);
                atGen(f, shell, contextptr);
                shell.editParen(RPAREN);
            } else
                atGen(f, shell, contextptr);
            continue;
        }

        if (f._SYMBptr->sommet == giac::at_inv) {
            giac::gen g(f._SYMBptr->feuille);
            if (i) {
                shell.editOperator('*', QString("×"));
                shell.editFrac();
                shell.editChar('1');
                shell.editMoveDown();
                atGen(g, shell, contextptr);
                shell.editMoveRight();
            } else
                atGen(f, shell, contextptr);
        } else {
            if (i)
                shell.editOperator('*', QString("×"));

            if (f._SYMBptr->sommet == giac::at_plus
                    || f._SYMBptr->sommet == giac::at_neg
                    || giac::is_inequation(f)) {
                shell.editParen(LPAREN);
                atGen(f, shell, contextptr);
                shell.editParen(RPAREN);
            } else
                atGen(f, shell, contextptr);
        }
    }
    return;
}

void atInv(const giac::gen &feuille, EditionTree &shell, EXT_GIAC_CONTEXT)
{
    std::cout << "@atInv\n";
    giac::gen f = feuille;
    bool isneg = false;
    bool calc38 = giac::calc_mode(contextptr) == 38;

    if (f.is_symb_of_sommet(giac::at_neg) && f._SYMBptr->feuille.type != giac::_VECT) {
        f = f._SYMBptr->feuille;
        isneg = true;
    }

    if (f.type < giac::_CPLX && giac::is_positive(-f, contextptr)) {
        f = -f;
        isneg = !isneg;
    }

    int l = (int) shell.empty();
    char lastEdition = shell.getLastEdition();
    if (isneg && l && lastEdition == '-') {
        shell.editDelete();
        shell.editOperator('+', "+");
        isneg = false;
    }
    if (isneg && l && lastEdition == '+') {
        shell.editDelete();
        shell.editOperator('+', QString("－"));
        isneg = false;
    }

    shell.editFrac();
    if (isneg)
        shell.editOperator('-', calc38 ? QString("−") : QString("-"));
    shell.editChar('1');
    shell.editMoveDown();
    atGen(f, shell, contextptr);
    shell.editMoveRight();
    return;
}

void atSPrint(const giac::gen &e, EditionTree &shell, EXT_GIAC_CONTEXT)
// Je ne comprends pas, dans symbolic.cc, pourquoi il y a un add_print
// ET un add_print_symbolic... quelle est la différence ?
{
    std::cout << "@atSPrint\n";
    if (e.type == giac::_IDNT) {
        /*
        if (calc_mode(contextptr)==1 && (is_inf(g) || is_undef(g)))
            s += "?";
        else
        (s += g._IDNTptr->print(contextptr));
        return s;
        */
        return;
    }

    // int l = s.size();

    if (e.type == giac::_INT_ && e.subtype == 0)
        atInt(e.val, shell, contextptr);
    if (e.type == giac::_VECT && e.subtype == 0) {
        /* Pure vectors are unsupported by edition trees. */
        // s += calc_mode(contextptr)==1?'{':'[';
        // add_printinner_VECT(s,*g._VECTptr,0,contextptr);
        // s += calc_mode(contextptr)==1?'}':']';
        // return s;
        return;
    }
    if (e.type == giac::_FRAC && e._FRACptr->num.type == giac::_INT_
            && e._FRACptr->den.type == giac::_INT_) {
        shell.editFrac(); // TODO : force non-absorption
        atGen(e._FRACptr->num, shell, contextptr);
        shell.editMoveDown();
        atGen(e._FRACptr->den, shell, contextptr);
        return;
    }
    if (e.type == giac::_SYMB) {
        if (e.subtype == giac::_SPREAD__SYMB)
            shell.editOperator('=', "=");
        return atSymbolic(*e._SYMBptr, shell, contextptr);
    }

    /* À partir de là, il faudrait gérer le contextptr, mais je ne sais pas quoi
     * en faire... */


//#ifdef EMCC
    const std::string tmp = e.print(contextptr);
// #else
//     const string & tmp=g.print(contextptr);
// #endif
    // check +- -> - and -- -> +
    if (!shell.empty() && shell.getLastEdition() == '+') {
        if (!tmp.empty() && tmp[0] == '-') {
            shell.editDelete();
            atGen(e, shell, contextptr);
            return;
        }
    }
//      if (!tmp.empty() && tmp[0]=='-'){
//  s = s.substr(0,l-1);
//  s += tmp;
//  return s;
//      }
//      if (tmp.size()>3 && (unsigned char)tmp[0]==226 && (unsigned char)tmp[1]==136 && (unsigned char) tmp[2]==146 ) { // -30, -120, -110
//  s[l-1]='-';
//  s += tmp.substr(3,tmp.size()-3);
//  return s;
//      }
//    }
//    if (l && s[l-1]=='-' ){
//      if (!tmp.empty() && tmp[0]=='-'){
//  s[l-1]='+';
//  s += tmp.substr(1,tmp.size()-1);
//  return s;
//      }
//      if (tmp.size()>3 && (unsigned char)tmp[0]==226 && (unsigned char)tmp[1]==136 && (unsigned char) tmp[2]==146 ) { // -30, -120, -110
//  s[l-1]='+';
//  s += tmp.substr(3,tmp.size()-3);
//  return s;
//      }
//    }
//    s += tmp;
//    return s;
}


void atSymbolic(const giac::symbolic &e, EditionTree &shell, EXT_GIAC_CONTEXT)
{
    std::cout << "@atSymbolic " << e << "\n";
    if (!e.sommet.ptr()) {
        shell.editStr("NULL");
        shell.editParen(LPAREN);
        atGen(e.feuille, shell, contextptr);
        shell.editParen(RPAREN);
        return;
    }

    if (e.sommet.ptr()->printsommet
            && e.sommet != giac::at_plus && e.sommet != giac::at_prod
            && e.sommet != giac::at_pow) {
        std::cout << "printsommet";
        // s += g.sommet.ptr()->printsommet(g.feuille,g.sommet.ptr()->s,contextptr);
        shell.editStr(e.sommet.ptr()->s);
        atGen(e.feuille, shell, contextptr);
        return;
    }

    if (e.feuille.type == giac::_VECT && e.feuille._VECTptr->empty())
        // Il faut gérer le contextptr
        // s += g.sommet.ptr()->print(contextptr);
        // s += "(NULL)";
        // return s;
        return;

    std::cout << "->@atSymbolic : entering operators...\n";
    if (e.sommet == giac::at_prod)
        return atProd(e, shell, contextptr);
    if (e.sommet == giac::at_plus)
        return atPlus(e, shell, contextptr);
    if (e.sommet == giac::at_pow)
        // return atPow(e, shell);
        return;
    if (e.sommet == giac::at_neg)
        return atNeg(e.feuille, shell, contextptr);
    if (e.sommet == giac::at_inv)
        return atInv(e.feuille, shell, contextptr);
    if (e.sommet == giac::at_exp)
        return;
//#ifndef EMCC
//  && (calc_mode(contextptr)==1 || abs_calc_mode(contextptr)==38)
//#endif
//  ){
//      s += printasexp(g.feuille,0,contextptr);
//      return s;
//    }
    return;
//    if ( g.feuille.type!=_VECT || ( g.sommet!=at_prod && g.feuille._VECTptr->front().type==_VECT ) ){
//      s += g.sommet.ptr()->print(contextptr);
//      s += '(';
//      add_print(s,g.feuille,contextptr);
//      s += ')';
//      return s;
//    }
//    int l=int(g.feuille._VECTptr->size());
//    s += g.sommet.ptr()->print(contextptr);
//    s += '(';
//    if (g.feuille.subtype!=_SEQ__VECT)
//      s += begin_VECT_string(g.feuille.subtype,false,contextptr);
//    for (int i=0;;++i){
//      add_print(s,(*(g.feuille._VECTptr))[i],contextptr); // s += (*(feuille._VECTptr))[i].print(contextptr);
//      if (i==l-1){
//  break;
//      }
//      s += ',';
//    }
//    if (g.feuille.subtype!=_SEQ__VECT)
//      s += end_VECT_string(g.feuille.subtype,false,contextptr);
//    s += ')';
//    return s;
    std::cout << "-> @atSymbolic : UNTREATED CASE\n";
    return;
}

void atGen(const giac::gen &e, EditionTree &shell, EXT_GIAC_CONTEXT)
{
    std::cout << "@atGen\n";
    std::cout << "GENERATING " << e << std::endl;
    switch (e.type) {
    case giac::_INT_:
        atInt(e.val, shell, contextptr);
        break;
    case giac::_DOUBLE_:
        atLeaf(e, shell, contextptr);
        break;
    case giac::_ZINT:
        atLeaf(e, shell, contextptr);
        break;
    case giac::_CPLX:
        std::cout << "_CPLX" << std::endl;
        break;
    case giac::_IDNT:
        std::cout << "_IDNT" << std::endl;
        break;
    case giac::_SYMB:
        std::cout << "_SYMB" << std::endl;
        if (giac::is_inf(e._SYMBptr->feuille)) {
            if (e._SYMBptr->sommet == giac::at_plus)
                shell.editChar('?'); // TODO infinity
            else if (e._SYMBptr->sommet == giac::at_neg) {
                shell.editOperator('-', "-");
                shell.editChar('?'); // TODO infinity
            }
            return;
        }

        if (e.subtype == giac::_SPREAD__SYMB) {
            if (e._SYMBptr->sommet == giac::at_sto)
                shell.editStr("=(CONTEXT_PTR)");
            else
                shell.editStr("=CONTEXT_PTR");
            return;
        } else
            return atSymbolic((*e._SYMBptr), shell, contextptr);
        break;
    case giac::_VECT:
        std::cout << "_VECT" << std::endl;
        break;
    case giac::_FUNC:
        std::cout << "_FUNC" << std::endl;
        break;
    default:
        std::cout << "Unknown case atGen" << std::endl;
        break;
    }
}

void copyExprAtCursor(giac::gen &expr, EditionTree &shell)
{
    std::cout << "@copyExprAtCursor\n";
    std::cout << "... parsing GIAC=" << expr << std::endl;
    atGen(expr, shell, giac::context0);
}

