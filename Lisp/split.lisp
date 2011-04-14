(defun my-split (x list) 
  (let ((fst (first list))
       (rst (rest list)))
       (if (endp list)
         '(() ())
         (cond 
           ((< fst x)
            (list
              (cons
                fst 
                (first (my-split x rst)))
              (second (my-split x rst))))
           ((> fst x)
            (list
              (first (my-split x rst))
              (cons 
                fst 
                (second (my-split x rst)))))
           ((= fst x)
            (my-split x rst))))))

