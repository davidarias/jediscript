;; the tradicional hello world
(print "Hello world!")

;; print the jedi code ;)
(new-line)
(the-jedi-code)

;; MATH
(new-line)
(print "Some math:")
(print "----------")
(print "(+ 2 (* 3 (/ 20 (% 11 2)))) => " (+ 2 (* 3 (/ 20 (% 11 2)))))
(print "(sqrt 2) => " (sqrt 2))


;; IF expressions
(new-line)
(print "Some if expressions:")
(print "----------")
(if (> 3  3)  (print "true!!") (print "false!!"))
(if (== 3  3)  (print "true!!") (print "false!!"))


;; Defines
(new-line)
(print "Defines:")
(print "----------")
(def number -3.45)
(print "number is: " number)


;; Functions
(new-line)
(print "Functions")
(print "----------")
(defn double [a]
  (* a 2))
(print "(double 2) => " (double 2))


;; Let
(new-line)
(print "let scope")
(print "----------")
(print (let [x (* 3 4)
             y (sqrt 5)]
         (floor (+ x y))))


;; while
(new-line)
(print "while")
(print "----------")
(print (let [i 0]
         (while (< i 10)
           (print "i: " i)
           (set! i (inc i)))))

;; Quote expressions
(new-line)
(print "Quote expressions!")
(print "(quote (z 5))) => " (quote (z 5)))
(print "'(z 5) => " '(z 5))
(print "`(1 2 ~@(list 3 4))) => " `(1 2 ~@(list 3 4)))
(print "`(1 2 ~(list 3 4))) => " `(1 2 ~(list 3 4)))

;; Macros!
(new-line)
(print "Macros!")
(print "----------")
(print "macroexpand (def d 3) => " (macroexpand '(def d 3)))

;; actually this macro is defined in the core lib
(defmacro apply [fn args]
  `(~fn ~@args))

(print "macroexpand (apply + [3 4 5]) => " (macroexpand '(apply + [3 4 5])))


(new-line)
(new-line)
;; load and execure test:
(print "Load tests: ")
(load-script "scripts/tests.cljs")
