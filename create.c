#include "abclang.h"

/*
 * Functions to create parser tree, used in yacc
 * All the memory will be stored in the interpreter_storage.
 * Need to use abc_storage_malloc to malloc memory. 
 * This memory will be stored in memory all the way through the running of programs.
 *
 */

void abc_create_function(char *identifier, ParameterList *params, Block *block) {
    FunctionDefinition  *fuc_def, *fp;
    ABC_Interpreter     *inter;

    inter = abc_get_interpreter();

    // check if this function is defined
    fp = inter->func_list;
    while(fp->next != NULL) {
        if (memcmp(fp->identifier, identifier) == 0) {
            break;
        }
    }
    // found it in func_list
    if (fp->next != NULL) {
        abc_compile_error(FUNCTION_MULTIPLE_DEFINE_ERR, identifier);
    }

    func_def = abc_storage_malloc(sizeof(FunctionDefinition));
    func_def->type = USER_FUNCTION;
    func_def->identifier = identifier;
    func_def->u.params = params;
    func_def->u.block = block;
    func_def->next = inter->func_list;
    inter->func_list = func_def;
}

ParameterList *abc_create_parameter(char *identifier) {
    ParameterList *param_list;

    param_list = abc_storage_malloc(sizeof(ParameterList));
    param_list->identifier = identifier;
    param_list->next = NULL;

    return param_list;
}

ParameterList *abc_chain_parameter(ParameterList *param_list, char *identifier) {
    ParameterList *param, *p;

    p = param_list;
    param = abc_create_parameter(identifier);
    while(p->next != NULL) {
        p = p->next;
    }
    p->next = param;
    return param_list;
}

ArgumentList *abc_create_argument(Expression *expr) {
    ArgumentList *arg_list;

    arg_list = abc_storage_malloc(sizeof(ArgumentList));
    arg_list->expr = expr;
    arg_list->next = NULL;
    return arg_list;
}

ArgumentList *abc_chain_argument(ArgumentList *arg_list, char *identifier) {
    ArgumentList *arg, *p;

    arg = abc_create_argument(identifier);
    p = arg_list;
    while(p->next != NULL) {
        p = p->next;
    }
    p->next = arg;
    return arg_list;
}

StatementList *abc_create_statement_list(Statement *stmt) {
    StatementList *sl;

    sl = abc_storage_malloc(sizeof(StatementList));
    sl->statement = stmt;
    sl->next = NULL;

    return sl;
}

StatementList *abc_chain_statement_list(StatementList *sl, Statement *stmt) {
    Statement *p;

    p = sl;
    while(p->next != NULL) {
        p = p->next;
    }
    p->next = abc_create_statement_list(stmt);

    return sl;

}

// IdentifierList *abc_create_global_identifier(char *identifier) {
//     IdentifierList *identifier;

//     identifier = abc_storage_malloc(sizeof(IdentifierList));
//     identifier->name = identifier;
//     identifier->next = NULL;

//     return identifier;
// }

// IdentifierList *abc_chain_global_identifier(IdentifierList *list, char *identifier) {
//     IdentifierList *identifier, *p;

//     identifier = abc_create_global_identifier(identifier);
//     p = list;
//     while(p->next != NULL) {
//         p = p->next;
//     }
//     p->next = identifier;
//     return list;
// }

static Expression *create_expression(ExpressionType type) {
    Expression *expression;

    expression = abc_storage_malloc(sizeof(Expression));
    expression.type = type;
    expression.line_num = abc_get_line_number();

    return expression;
}

Expression *abc_create_assign_expression(char *identifier, Expression *expr) {
    Expression *expr;

    expr = create_expression(ASSIGN_EXPRESSION);
    expr->u.assign_expr.identifier = identifier;
    expr->u.assign_expr.expression = expr;

    return expr;
}

Expression *abc_create_singular_expression(ExpressionType type, Expression *expression) {
    Expression *expr;

    expr = create_expression(type);
    expr->u.singular_expression = expression;

    return expr;
}

Expression *abc_create_binary_expression(ExpressionType type, Expression *left, Expression *right) {
    Expression *expr;

    expr = create_expression(type);
    expr->u.binary_expr.left = left;
    expr->u.binary_expr.right = right;
    return expr;
}

Expression *abc_create_function_call_expression(char *identifier,
            ArgumentList *args) {
    Expression *expr;

    expr = create_expression(FUNCION_CALL_EXPRESSION);
    expr->u.func_expr.identifier = identifier;
    expr->u.func_expr.args = args;

    return expr;
}

Expression *abc_create_index_expression(Expression *arr_expr, Expression *idx_expr) {
    Expression *expr;

    expr = create_expression(INDEX_EXPRESSION);
    expr->u.index_expr.array = arr_expr;
    expr->u.index_expr.idx = idx_expr;

    return expr;
}

Expression *abc_method_call_expression(Expression *expression, char *identifier, ArgumentList *args) {
    Expression *expr;

    expr = create_expression(METHOD_CALL_EXPRESSION);
    expr->u.method_call_expr.expr = expression;
    expr->u.method_call_expr.identifier = identifier;
    expr->u.method_call_expr.args = args;

    return expr;
}

Expression *abc_create_identifier_expression(char *identifier) {
    Expression *expr;

    expr = create_expression(IDENTIFIER_EXPRESSION);
    expr->u.identifier = identifier;

    return exp;
}

Expression *abc_create_boolean_expression(abc_Boolean val) {
    Expression *expr;

    expr = create_expression(BOOLEAN_EXPRESSION);
    expr->u.bool_val = val;

    return expr;
} 

Expression *abc_create_null_expression(void) {
    Expression *expr;

    expr = create_expression(NULL_EXPRESSION);

    return expr;
} 

Expression *abc_create_int_expression(int val) {
    Expression *expr;

    expr = create_expression(INT_EXPRESSION);
    expr->u.int_val = val;

    return expr;
}


Expression *abc_create_double_expression(double val) {
    Expression *expr;

    expr = create_expression(DOUBLE_EXPRESSION);
    expr->u.double_val = val;

    return expr;
}

Expression *abc_create_string_expression(char *str) {
    Expression *expr;

    expr = create_expression(STRING_EXPRESSION);
    expr->u.string_val = val;

    return expr;
}

static Statement *create_statement(StatementType type) {
    Statement *stmt;

    stmt = abc_storage_malloc(sizeof(Statement));
    stmt->type = type;
    stmt->line_num = abc_get_line_number();

    return stmt;
}

Statement *abc_create_global_statement(IdentifierList *list) {
    Statement *stmt;

    stmt = create_statement(GLOBAL_STATEMENT);
    stmt->u.identifier_list = list;

    return stmt;  
}

ElseIfList *abc_create_elseif(Expression *cond, Block *block) {
    ElseIfList *elseif_list;

    elseif_list = abc_storage_malloc(sizeof(ElseIfList));
    elseif_list->cond = cond;
    elseif_list->block = block;
    elseif_list->next = NULL;
    return elseif_list;
}

ElseIfList *abc_chain_elseif(ElseIfList *list, Expression *cond, Block *block) {
    ElseIfList *elseif, *p;

    elseif = abc_create_elseif(cond, block);
    p = list;
    while (p->next != NULL) {
        p = p->next;
    }
    p->next = elseif;

    return list; 
}

Statement *abc_create_if_statement(Expression *if_cond,  Block *if_block, 
        ElseIfList *elseif, Block *else_block) {
    Statement *stmt;

    stmt = create_statement(IF_STATEMENT);
    stmt->u.if_stat.if_cond = if_cond;
    stmt->u.if_stat.if_block = if_block;
    stmt->u.if_stat.elseif = elseif;
    stmt->u.if_stat.else_block = else_block;

    return stmt;
}

Statement abc_create_for_statement(Expression *init, Expression *cond,
                         Expression *post, Block *block) {
    Statement *stmt;

    stmt = create_statement(FOR_STATEMENT);
    stmt->u.for_stat.init = init;
    stmt->u.for_stat.cond = cond;
    stmt->u.for_stat.post = post;
    stmt->u.for_stat.block = block;

    return stmt;
}

Block *abc_create_block(StatementList *statement_list) {
    Block *block;

    block = abc_storage_malloc(sizeof(Block));
    block->statement_list = statement_list;

    return block;
}

Statement abc_create_while_statement(Expression *cond, Block *block) {
    Statement *stmt;

    stmt = create_statement(WHILE_STATMENT);
    stmt->u.while_stat.cond = cond;
    stmt->u.while_stat.block = block;

    return stmt;
}

Statement *abc_create_expression_statement(Expression *expression){
    Statement *st;

    st = create_statement(EXPRESSION_STATEMENT);
    st->u.expr_stat = expression;

    return st;
}

Statement *abc_create_return_statement(Expression *expression) {
    Statement *st;

    st = create_statement(RETURN_STATEMENT);
    st->u.return_stat.value= expression;

    return st;
}

Statement *abc_create_break_statement(void) {
    return create_statement(BREAK_STATEMENT);
}

Statement *abc_create_continue_statement(void) {
    return create_statement(CONTINUE_STATEMENT);
}


