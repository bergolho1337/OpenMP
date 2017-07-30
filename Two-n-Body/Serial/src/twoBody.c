#include "../include/twoBody.h"

void readInput (TwoBody *m)
{
    if (!scanf("%lf %d %d %d",&m->dt,&m->num_steps,&m->out_freq,&m->n)) printError("Reading input");
    m->p = (Particle*)malloc(sizeof(Particle)*m->n);
    for (int i = 0; i < m->n; i++)
    {
        if (!scanf("%lf %lf %lf %lf %lf %lf %lf",&m->p[i].mass,&m->p[i].pos[X],&m->p[i].pos[Y],&m->p[i].pos[Z],\
        &m->p[i].vel[X],&m->p[i].vel[Y],&m->p[i].vel[Z])) printError("Reading input");
    }
        
}

void printError (const char msg[])
{
    fprintf(stderr,"[-] ERROR ! %s\n",msg);
    exit(EXIT_FAILURE);
}

void printParticle (const Particle *p, int N)
{
    for (int i = 0; i < N; i++)
    {
        printf("%.3e\t%.3e\t%.3e\t%.3e\t%.3e\t%.3e\n",p[i].pos[X],p[i].pos[Y],p[i].pos[Z],p[i].vel[X],p[i].vel[Y],p[i].vel[Z]);
    }
    
}

void setParticle (Particle &p, double m, double x, double y, double z, double vx, double vy, double vz, double ax, double ay, double az)
{
    p.mass = m;
    p.pos[X] = x; p.pos[Y] = y; p.pos[Z] = z;
    p.vel[X] = vx; p.vel[Y] = vy; p.vel[Z] = z;
}

TwoBody* newTwoBody ()
{
    TwoBody *model = (TwoBody*)malloc(sizeof(TwoBody));
    readInput(model);
    //printParticle(model->p,model->n);
    return model;
}

/* Compute the next postion and velocity of the particle by using Explicit Euler */
void Compute_pos_vel (Particle *p, vect_t *forces, double dt, int q, int n)
{
    p[q].pos[X] += dt*p[q].vel[X];
    p[q].pos[Y] += dt*p[q].vel[Y];
    p[q].pos[Z] += dt*p[q].vel[Z];

    /* Second Newton Law */
    p[q].vel[X] += dt/p[q].mass*forces[q][X];
    p[q].vel[Y] += dt/p[q].mass*forces[q][Y];
    p[q].vel[Z] += dt/p[q].mass*forces[q][Z];
}

/* Compute the forces using the reduced algorithm of the Two-N-Body */
void Compute_force (Particle *p, vect_t *forces, int q, int n)
{
    int k;
    double x_diff, y_diff, z_diff, dist, dist_cubed;
    vect_t forces_qk;

    /* Remenber that the forces: f_qk = -f_kq (Third Newton Law) */
    for (k = q+1; k < n; k++)
    {
        x_diff = p[q].pos[X] - p[k].pos[X];
        y_diff = p[q].pos[Y] - p[k].pos[Y];
        z_diff = p[q].pos[Z] - p[k].pos[Z];
        dist = sqrt(x_diff*x_diff + y_diff*y_diff + z_diff*z_diff);
        dist_cubed = dist*dist*dist;
        forces_qk[X] = -G*p[q].mass*p[k].mass / dist_cubed * x_diff;
        forces_qk[Y] = -G*p[q].mass*p[k].mass / dist_cubed * y_diff;
        forces_qk[Z] = -G*p[q].mass*p[k].mass / dist_cubed * z_diff;

        forces[q][X] += forces_qk[X];
        forces[q][Y] += forces_qk[Y];
        forces[q][Z] += forces_qk[Z];
        forces[k][X] -= forces_qk[X];
        forces[k][Y] -= forces_qk[Y];
        forces[k][Z] -= forces_qk[Z];
    }

    

}

void solveModel (TwoBody *m)
{
    int step, part;
    double t;
    vect_t *forces = (vect_t*)malloc(sizeof(vect_t)*m->n);
    Particle *p = m->p;

    // Time loop
    for (step = 0; step < m->num_steps; step++)
    {
        t = step*m->dt;

        /* Assign 0 to each element of the forces array */
        memset(forces,0,sizeof(vect_t)*m->n);
    
        /* For each particle compute the force */
        for (part = 0; part < m->n; part++)
            Compute_force(p,forces,part,m->n);

        /* For each particle compute the position and the velocity */
        for (part = 0; part < m->n; part++)
            Compute_pos_vel(p,forces,m->dt,part,m->n);

        if (step % m->out_freq == 0)
            writeVTK(step,p,m->n);
            
    }
    free(forces);
}

void writeVTK (int step, Particle *p, int n)
{
    int i;
    char filename[500];
    sprintf(filename,"VTK/step%d.vtk",step);
    FILE *vtk = fopen(filename,"w+");
    fprintf(vtk,"# vtk DataFile Version 3.0\n");
    fprintf(vtk,"Vtk output\n");
    fprintf(vtk,"ASCII\n");
    fprintf(vtk,"DATASET POLYDATA\n");
    fprintf(vtk,"POINTS %d float\n",n);
    for (i = 0; i < n; i++)
        fprintf(vtk,"%e %e %e\n",p[i].pos[X],p[i].pos[Y],p[i].pos[Z]);
    fprintf(vtk,"VERTICES %d %d\n",n,n*2);
    for (i = 0; i < n; i++)
        fprintf(vtk,"1 %d\n",i);
    fclose(vtk);
}
