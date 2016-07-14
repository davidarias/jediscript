(defn test [name expresion expected-result]
  (if (== expresion expected-result)
     (print "[OK]   " name)
     (print "[FAIL] " name))
