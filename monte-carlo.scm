;estimating pi by monte carlo
;(the probability that two random integers will be coprime is equal to :
;(/ 6 (square pi))

(define rand (lambda () (random 10000)))
(define (random-in-range low high) 
	(let ((range (- high low))) (+ low (random range))))

(define (estimate-pi trials)
	(sqrt (/ 6 (monte-carlo trials cesaro-test))))

(define (cesaro-test) (= (gcd (rand) (rand)) 1))

(define (monte-carlo trials experiment)
	(define (iter trials-remaining trials-passed)
		(cond ((= trials-remaining 0) (/ trials-passed trials))
					((experiment) (iter (- trials-remaining 1) (+ trials-passed 1)))
					(else (iter (- trials-remaining 1) trials-passed))))
	(iter trials 0))

;monte carlo integration

(define (estimate-integral p x1 x2 y1 y2 trials)
	(define (experiment) (p (random-in-range x1 x2) (random-in-range y1 y2)))
	(* (* (- x2 x1) (- y2 y1)) (monte-carlo trials experiment)))

;(define est-pi (estimate-integral (lambda (x y) (<= (+ (* x x) (* y y)) 100)) -100 100 -100 100 1000000))
