function op(o) {
    return {type: "op", op: o}
}

function variable(o) {
    return {type: "var", name: o}
}

function node(type, children) {
    return {type: type, content: children}
}

function set_cursor(node, pos) {
    if (pos)
        node.cursor = pos;
    else
        node.cursor = 0;
    return node;
}

function sqrt(flow) {
    return node("sqrt", [flow]);
}

function pow(flow) {
    return node("pow", [flow]);
}

function frac(num, den) {
    return node("frac", [num, den]);
}

function sum(from, to) {
    return node("sum", [from, to]);
}

function lparen() {
    return {type: "lparen"}
}

function rparen() {
    return {type: "rparen"}
}

function selected_text(text, pos) {
    return {type: "selected_text", text: text, cursor: pos}
}
