:-dynamic(room/4).
:-dynamic(instructor/3).
:-dynamic(student/3).
:-dynamic(course/6).
:-dynamic(occupancy/3).

%room(roomID, capacity, equipment, hours)
room(z01, 20, [projector, sBoard, hCapped], 9).     %Meaning 8 am to 17pm availaible
room(z23, 15, [projector, hCapped], 9).             %Meaning 8 am to 17pm availaible
room(z11, 10, [sBoard, hCapped], 7).                %Meaning 8 am to 15pm availaible
room(z10, 10, [projector], 5).                      %Meaning 8 am to 13pm availaible
room(z07, 10, [sBoard, hCapped], 5).                %Meaning 8 am to 13pm availaible
room(z03, 5, [], 3).                                %Meaning 8am to 11am availaible

%instructor(instructorID, classes, preferences).
instructor(01, [cse322, cse221, cse222], [projector]).
instructor(02, [cse420, cse320, cse220], [sBoard]).
instructor(03, [cse121, cse122, cse421, cse422], []).
instructor(04, [cse120, cse321], []).

%student(studentID,courses,hCapped)
%student(studentID,courses,hCapped)
student(001, [cse120, cse121, cse122], no).
student(002, [cse220, cse221, cse222], hCapped).
student(003, [cse220, cse320, cse321], no).
student(004, [cse420, cse421, cse422], no).
student(005, [cse322, cse221], hCapped).
student(006, [cse121, cse122], no).
student(007, [cse220, cse221, cse222], no).
student(008, [cse421, cse321, cse420, cse422], no).
student(009, [cse322], hCapped).

%course(courseID, instructorID, capacity, hour, room, needs)
course(cse120, 04, 10, 2, z10, [projector]).
course(cse121, 03, 5, 1, z03, []).
course(cse122, 03, 5, 1, z03, []).
course(cse220, 02, 10, 3, z11, [sBoard, hCapped]).
course(cse221, 01, 15, 3, z23, [projector, hCapped]).
course(cse222, 01, 10, 2, z23, [projector, hCapped]).
course(cse320, 02, 5, 1, z11, [sBoard]).
course(cse321, 04, 10, 2, z10, []).
course(cse322, 01, 20, 3, z01, [sBoard, hCapped]).
course(cse420, 02, 10, 2, z07, [sBoard]).
course(cse421, 03, 20, 2, z01, []).
course(cse422, 03, 10, 2, z07, []).

%occupancy(roomID, hour, courseID)
occupancy(z01, 10, cse322).
occupancy(z01, 11, cse322).
occupancy(z01, 12, cse322).
occupancy(z01, 8, cse421).
occupancy(z01, 9, cse421).
occupancy(z03, 8, cse121).
occupancy(z03, 10, cse122).
occupancy(z07, 8, cse420).
occupancy(z07, 9, cse420).
occupancy(z07, 10, cse422).
occupancy(z07, 11, cse422).
occupancy(z10, 8, cse120).
occupancy(z10, 9, cse120).
occupancy(z10, 10, cse321).
occupancy(z10, 11, cse321).
occupancy(z11, 8, cse220).
occupancy(z11, 9, cse220).
occupancy(z11, 10, cse320).
occupancy(z23, 10, cse221).
occupancy(z23, 11, cse221).
occupancy(z23, 12, cse221).
occupancy(z23, 8, cse222).
occupancy(z23, 9, cse222).

%queries
%Check whether there is any courses that are assigned to the same room at the same hour/hours.
%A and B should be courseID
conflictCourses(A,B):-
    occupancy(X, Y, A), occupancy(K,M,B), X = K, Y is M.

%Check whether if a room is assigned to 2 different classes at the same hour.
%A is roomID, B is hour.
conflictOccupancy(A,B):-
	occupancy(A, B, X), occupancy(A, B, Y), X\=Y.

%Check if the two courses are in the same hours.
%A and B should be courseID
conflictStudentCourses(A,B):-
	occupancy(_,Y,A), occupancy(_,M,B), Y is M.

%Check which room can be assigned to which class/classes.
%A should be a courseID or it can be passed as an unknwon variable as well.
checkAssignClass(A):-
    course(A,_,_,H,_,EList),
    room(RID,_,ERList,RH),
    occupancy(RID,OH,_),
    (include_lists(EList, ERList)->
  		((OH\=RH+8-1)->
        	(RH+8-1-OH>=H ->
            	(incr(OH,M), not(occupancy(RID, M,_))
                ->   write(RID), write(" Can be chosen for "), write(A))
            )
        )
    ).

%Check which classes a student/students take.
%A should be a studentID or it can be passed as an unknwon variable as well.
checkEnrollStudent(A):-
    student(A,_,H),
    course(CID,_,C,_,R,_),
    room(R,_,EList,_),
    (course_counterr(CID,Res), Res<C ->
    	((member(H, EList);H==no) ->  write(A), write(" can enroll to "), write(CID)
        )
    ).

%Check if a student can enroll to a course.
%A is studentID, B is courseID
checkStudentCourse(A,B):-
    student(A,_,H),
    course(B,_,C,_,R,_),
    room(R,_,EList,_),
    (course_counterr(B,Res), Res<C ->
    	((member(H, EList);H==no)
        )
    ).

%Check if a room can be assigned to a course
%A is course, B is room
checkRoomCourse(A,B):-
    course(A,_,_,H,_,EList),
    room(B,_,ERList,RH),
    occupancy(B,OH,_),
    (include_lists(EList, ERList)->
  		((OH\=RH+8-1)->
        	(RH+8-1-OH>=H ->
            	(incr(OH,M), not(occupancy(B, M,_))
                )
            )
        )
    ).

%Add a new room
%RID is room id, C is capacity, E is needs, H is hours
addRoom(RID,C,E,H):-
    \+ room(RID,_,_,_), H=<9,
    assertz(room(RID,C,E,H)).

%Add a new course
%CID is course id, IID is instructor id, C is capacity, H is hours, R is room id, E is needs
addCourse(CID,IID,C,H,R,E):-
    \+ course(CID,_,_,_,_,_),
    instructor(IID,_,_),
    assertz(course(CID,IID,C,H,R,E)),
    ( not(checkRoomCourse(CID,R) )->  retract(course(CID,IID,C,H,R,E)),write("Course can't be added")
    ;   write("Course added")).

%Add a new student
%SID is student id, Cours is the list of taken courses, H is for hCapped.
addStudent(SID,Cours,H):-
    \+ student(SID,_,_),
    checkCourseValidity(Cours),
    not(checkCourseOverlapping(Cours)),
    assertz(student(SID,Cours,H)),
    (checkAvailibilityForAll(SID,Cours);retract(student(SID,Cours,H)),1 == 2).

%Helper methods
list_empty([], true).
list_empty([_|_], false).

checkCourseOverlapping([]).
checkCourseOverlapping([H|T]):-
    list_empty(T,R),R==false,
    (not(searchConflict(H,T))-> checkCourseOverlapping(T); true).

searchConflict(_,[]).
searchConflict(C,[H|T]):-
    (list_empty(T,R), R==true -> conflictStudentCourses(C,H);
    (not(conflictStudentCourses(C,H))->searchConflict(C,T);true)).

checkCourseValidity([]).
checkCourseValidity([H|T]):-
    course(H,_,_,_,_,_),
    checkCourseValidity(T).

checkAvailibilityForAll(_,[]).
checkAvailibilityForAll(SID,[H|T]):-
    checkStudentCourse(SID,H),
    checkAvailibilityForAll(SID,T).

incr(X, X1) :-
    X1 is X+1.

include_lists(L1,L2):-
    length(L1,LL1),
    length(L2,LL2),
    LL1 =< LL2,
    include_backend(L1,L2).

include_backend([],_).
include_backend([H|T],L2):-
    member(H,L2),
    include_backend(T,L2).


course_counterr(CID,Res):- course_counterr(0,CID,[],X), length(X,Res),!.
course_counterr(100,_,Count,Count).
course_counterr(SID,CD1,Count,C):-
    SID < 100,
    student(SID,Courses,_),
    include_lists([CD1],Courses),

    (\+member(SID,Count),append(Count,[SID],Res);member(SID,Count)),
    SID1 is SID + 1,
    course_counterr(SID1,CD1,Res,C).
course_counterr(Hour,CID,Count,C):-
    Hour < 100,
    Hour1 is Hour + 1,
    course_counterr(Hour1,CID,Count,C).
