;; the function macro creates a macro from an arguments list and a body
;; defmacro is a convenient macro to use the primitive function macro
(define-symbol 'defmacro (macro [name args body]
                                `(define-symbol (quote ~name) (macro ~args ~body))))

;; another convenient macro to use the primitive define-symbol
(defmacro def [key value]
  `(define-symbol (quote ~key) ~value))

(defmacro set! [key value]
  `(set-symbol! (quote ~key) ~value))

(defmacro defn [name args body]
  `(def ~name (fn ~args ~body)))

(defmacro apply [fn args]
  `(~fn ~@args))

(defn != [a b]
  (not (== a b)))

(defn >= [a b]
  (or (> a b) (== a b)))

(defn <= [a b]
  (or (< a b) (== a b)))

(defn inc [n]
  (+ 1 n))

(defn dec [n]
  (- n 1))

(defmacro new-line []
  '(print))

;; some useful defs :)
(def tie-fighter "|-o-|")
(def death-star-defense-team "|-o-| (-o-) |-o-|")
(def table-flip "(╯°□°）╯ ︵ ┻━┻")
(def double-table-flip "┻━┻︵ \(°□°)/ ︵ ┻━┻ ")

(defn the-jedi-code []
  [(print "There is no emotion, there is peace.")
   (print "There is no ignorance, there is knowledge.")
   (print "There is no passion, there is serenity")
   (print "There is no chaos, there is harmony")
   (print "There is no death, there is the Force")])
