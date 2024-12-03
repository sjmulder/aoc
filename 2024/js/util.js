const indices = xs => [...xs.keys()];
const sum = xs => xs.reduce((a, b) => a + b);
const count = (xs, val) => xs.reduce(
    (acc, x) => acc += x == val ? 1 : 0, 0);
const countBy = (xs, pred) => xs.reduce(
    (acc, x) => acc += pred(x) ? 1 : 0, 0);

const someBy = (xs, pred) => {
    for (const x of xs) if (pred(x)) return true;
    return false;
};

const pairwise = xs => {
    const pairs = [];
    for (let i=1; i < xs.length; i++) pairs.push([xs[i-1], xs[i]]);
    return pairs;
};

module.exports = {indices, sum, count, countBy, someBy, pairwise};
