using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;

public class SelectPlayer : MonoBehaviour
{
    private bool firstStepDone=false;
    private bool selected=false;
    private int currentActionNo=0;
    public Animation anim;
    public Animator animtr;
    public AnimationClip[] clips;

    private void Start()
    {
        SwipeDetector.OnSwipe += SwipeDetector_OnSwipe;
        SwipeDetector1.OnSwipe += SwipeDetector1_OnSwipe;
        currentActionNo=0;
        selected=false;
        firstStepDone=false;
    }

    void Update () 
    {
         if (Input.touchCount > 0 && Input.GetTouch (0).phase == TouchPhase.Began) 
        {
            Ray ray = Camera.main.ScreenPointToRay( Input.GetTouch(0).position );
            RaycastHit hit;

            if (Physics.Raycast(ray, out hit) && hit.transform.gameObject.name == "player" && firstStepDone==true)
                selected=true;
        }
    }

    private void SwipeDetector_OnSwipe(SwipeData data)
    {
        if(data.Direction == SwipeDirection.Down)    //First Step at Selection
        {
            firstStepDone=true;
        }
        else if(data.Direction == SwipeDirection.Up)  //Clear Selection
        {
            firstStepDone=false;
            selected=false;
            currentActionNo=0;
            if(anim!=null)
            {
                anim.clip=clips[0];
                anim.Play();
            }
            else if(animtr!=null)
                animtr.Play("Fox_Idle");
        }
    }

    private void SwipeDetector1_OnSwipe(SwipeData data)
    {
        if (data.Direction == SwipeDirection.Up && currentActionNo==0 && selected==true)  //First Action
        {
            currentActionNo++;
            if(anim!=null)
            {
                anim.clip=clips[1];
                anim.Play();
            }
            else if(animtr!=null)
                animtr.Play("Fox_Sit_Idle_Break");
                
        } 
        else if(data.Direction == SwipeDirection.Left && currentActionNo==1 && selected==true)  //Second Action      
        {
            currentActionNo++;
            if(anim!=null)
            {
                anim.clip=clips[2];
                anim.Play();
            }
            else if(animtr!=null)
                animtr.Play("Fox_Somersault_InPlace");
        }
        else if(data.Direction == SwipeDirection.Right && currentActionNo==2 && selected==true) //Third Action
        {
            currentActionNo=0;
            if(anim!=null)
            {
                anim.clip=clips[3];
                anim.Play();
            }
            else if(animtr!=null)
                animtr.Play("Fox_Jump");
        }
    }
}
