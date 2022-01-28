Possible queries you can test:

*****For part 1*****
conflictCourses(cse221,cse222)		(Result should be false, no conflict)
conflictOccupancy(z23,10)		(Result should be false, no conflict)
conflictStudentCourses(cse221, cse222)	(Result should be false, no conflict)
conflictStudentCourses(cse221, cse321)	(Result should be true, they are at the same hour. A student cant choose both.)
checkAssignClass(cse321)	
Results should be:
z01 Can be chosen for cse321
true
z23 Can be chosen for cse321
true
z11 Can be chosen for cse321
true
z03 Can be chosen for cse321
true
checkAssignClass(X)
Results should be:
z01 Can be chosen for cse120
X = cse120
z23 Can be chosen for cse120
X = cse120
z01 Can be chosen for cse121
X = cse121
z23 Can be chosen for cse121
X = cse121
z11 Can be chosen for cse121
X = cse121
z10 Can be chosen for cse121
X = cse121
etc.
checkEnrollStudent(002)
Results should be:
2 can enroll to cse220
true
2 can enroll to cse221
true
2 can enroll to cse222
true
2 can enroll to cse320
true
2 can enroll to cse322
true
2 can enroll to cse420
true
2 can enroll to cse421
true
etc.
checkEnrollStudent(X)
Results should be:
1 can enroll to cse120
X = 1
1 can enroll to cse121
X = 1
1 can enroll to cse122
X = 1
1 can enroll to cse220
X = 1
1 can enroll to cse221
X = 1
etc.
checkStudentCourse(002,cse222)		(Result should be true, cse222 is available for handicapped)
checkStudentCourse(002,cse120)		(Result should be false, cse120 is not available for handicapped)
checkStudentCourse(X,Y)
Results should be:
X = 1,
Y = cse120
X = 1,
Y = cse121
X = 1,
Y = cse122
X = 1,
Y = cse220
X = 1,
Y = cse221
etc.
checkRoomCourse(cse420,z23)		(Result should be false, z23 does not have smart board)
checkRoomCourse(cse222,z23)		(Result should be true)
checkRoomCourse(A,B)
Results should be:
A = cse120,
B = z01
A = cse120,
B = z23
A = cse121,
B = z01
A = cse121,
B = z23
A = cse121,
B = z11
A = cse121,
B = z10
etc
addRoom(z23,5,[],5)			(Result should be false, z23 already exists)
addRoom(z33,5,[],10)			(Result should be false, a room cant be available for more than 9 hours 8am-5pm)
addRoom(z33,5,[projector,hCapped],5)	(Result should be true)
addStudent(001, [], no)			(Result should be false, student 001 already exists)
addStudent(011,[cse221,cse222],hCapped) (Result should be true)
addStudent(011,[cse221,cse321],hCapped) (Result should be false, cse221 and cse321 are at the same hours)
addCourse(cse520, 05, 10, 2, z23, [hCapped,sBoard])	(Result should be false, instructor 05 does not exist)
addCourse(cse520, 04, 10, 2, z23, [sBoard,hCapped])	(Result should be: Course can't be added because z23 does not have sBoard)
addCourse(cse520, 04, 10, 2, z01, [sBoard,hCapped])	(Result should be: Course added)

*****For part 2*****
?-route(van, ankara, 4)  (Result should be true)
?-route(van, ankara, 5)  (Result should be false)
?-route(rize,izmir,6)    (Result should be true)
?-route(rize,izmir,7)    (Result should be false)
?-route(rize,X,C) 	 
Results should be:
C = 4,
X = istanbul
C = 6,
X = izmir
C = 8,
X = antalya
C = 11,
X = erzincan
C = 17,
X = canakkale
C = 12,
X = diyarbakýr
C = 20,
X = ankara
C = 24,
X = van
C = 27,
X = gaziantep
C = 12,
X = ankara
etc.
?-route(Y,X,C)
Results should be:
C = 6,
X = erzincan,
Y = canakkale
C = 9,
X = antalya,
Y = canakkale
C = 11,
X = izmir,
Y = canakkale
C = 13,
X = istanbul,
Y = canakkale
C = 14,
X = ankara,
Y = canakkale
etc.