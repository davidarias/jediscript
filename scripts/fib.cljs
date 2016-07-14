(defn fib [x]
  (if (== x 0)
    0
    (if (== x 1)
      1
      (+ (fib (dec x)) (fib (- x 2)) ))))

(print (fib 10))
