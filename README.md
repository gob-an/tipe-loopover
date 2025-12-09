# Résolution du jeu loopover

résolution du jeu loopover (loopover.xyz), dans le cadre du TIPE, sur le thème "cycles et boucles", développé par carykh

## notes
metrics: utm/mtm
unit turn metric (single move)
multiple turn metrics (<1utm>)

mtm <= (s-1)/2 utm
8m max par 3c, 16 en utm
on décompose en produit de deux cycles de longueur paire:
* si impair, un n-cycle se décompo en (n-1)/2 3c
* si pair, un produit d'un n-cycle et d'un m-cycle se décompo en 1 3c + 1 (n+m-1)-cycle -> (n+m)/2 3c
dans tous les cas, un produit de deux cycles n et m se décomposent en (n+m)/2 3c
on obtient pour s la taille du plateau une majoration en s^2/2 3c
d'où 4s^2 en mtm 2s^2(s-1) en utm

signature préservée ssi s impaire
commutateurs -> 3c
d'où groupe alterné si impair, symétrique tout entier si pair


q: est-ce que si on se place dans le sous-groupe où le rectangle jusque (i,j) est résolu, la résolution en <Uk>k>=i <Rk>k>=j optimale?
-> non: eg 3c sur le côté
q: ok si uniformément mélangé?



## à faire
finir ida* -> min_heap
