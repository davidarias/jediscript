(load "test.cljs")

(new-line)
(print "Basic math:")
(print "-----------")
(test "plus"     (+ 23 67) 90)
(test "minus"    (- 1 3) -2)
(test "multiply" (* 20 5) 100)
(test "divide"   (/ 20 5) 4)
(test "module"   (% 11 2) 1)
(test "inc"      (inc 1) 2)
(test "dec"      (dec 2) 1)


(new-line)
(print "Logic:")
(print "-----------")
(test "and 1" (and true true) true)
(test "and 2" (and true false) false)
(test "and 3" (and false true) false)
(test "and 4" (and false false) false)

(test "or 1"  (or true true) true)
(test "or 2"  (or true false) true)
(test "or 3"  (or false true) true)
(test "or 4"  (or false false) false)

(new-line)
(print "Compare:")
(print "-----------")
(test "greater"            (> 345 56) true)
(test "greater 2"          (> 675 9231837) false)
(test "lesser"             (< -3455 56) true)
(test "lesser 2"           (< 23423 4) false)
(test "greater or equal"   (>= 345 56) true)
(test "greater or equal 2" (>= 675 675) true)
(test "lesser or equal"    (<= -3455 56) true)
(test "lesser or equal 2"  (<= 4 4) true)
(test "not equal"          (!= 4 6), true)
(test "not equal 2"        (!= 4 4), false)

(new-line)
(print "lists:")
(print "-----------")
(test "nth"            (nth 0 '( 0 1 2 3 4 )) 0)
(test "head"           (head '( 0 1 2 3 4 )) 0)
(test "tail"           (head (tail '( 0 1 2 3 4 ))) 1)
(test "tail 2"         (head (tail '( 3 4 ))) 4)
(test "tail 3"         (head (tail '( 4 ))) nil)
(test "push!"          (head (push! 3 '( ))) 3)
(test "pop!"           (pop! '( 5 )) 5)
(test "concat!"        (head (concat! '( 5 ) '( 6 ))) 5)
(test "apply"          (apply + [1 2 3]) 6)
(test "empty?"         (empty? '()) true)
(test "empty? 2"       (empty? '( 3 4 )) false)










