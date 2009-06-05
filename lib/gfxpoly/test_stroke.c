#include <math.h>
#include "../gfxtools.h"
#include "stroke.h"
#include "convert.h"

int main()
{
    gfxline_t l[512], f[256*5];

    int width=700,height=700;
    gfxdevice_t dev;
    gfxdevice_swf_init(&dev);
    dev.setparameter(&dev, "framerate", "25.0");
    int t;
    for(t=0;t<300;t++) {
	dev.startpage(&dev, 700,700);
	/*gfxline_t*g = l;
	while(g) {
	    g->x += g->sx;
	    g->y += g->sy;
	    if(g->sx || g->sy) {
		if(g->x<200) {g->x=400-g->x;g->sx=-g->sx;}
		if(g->y<200) {g->y=400-g->y;g->sy=-g->sy;}
		if(g->x>500) {g->x=1000-g->x;g->sx=-g->sx;}
		if(g->y>500) {g->y=1000-g->y;g->sy=-g->sy;}
	    }
	    g = g->next;
	}*/

	int i;
	for(i=0;i<512;i++) {
	    double a = i*0.05+t*M_PI/150;
	    double r = 50+i*0.5;
	    l[i].x = cos(a)*r + width/2;
	    l[i].y = sin(a)*r + height/2;
	    l[i].sx = (int)((l[i].x-width/2)/30);
	    l[i].sy = (int)((l[i].y-height/2)/30);
	    l[i].sx = l[i].sy = 0;
	    l[i].type = gfx_lineTo;
	    l[i].next = &l[i+1];
	}
	l[0].type = gfx_moveTo;
	l[i-1].next = 0;

	int xx,yy;
	i = 0;
	for(yy=0;yy<16;yy++) 
	for(xx=0;xx<16;xx++) if((xx^yy)&1) {
	    double x = -128+xx*64+t*128.0/300;
	    double y = -128+yy*64;//+t*64.0/300;
	    f[i].x = x;
	    f[i].y = y;
	    f[i].next = &f[i+1];f[i++].type = gfx_moveTo;
	    f[i].x = x+64;
	    f[i].y = y;
	    f[i].next = &f[i+1];f[i++].type = gfx_lineTo;
	    f[i].x = x+64;
	    f[i].y = y+64;
	    f[i].next = &f[i+1];f[i++].type = gfx_lineTo;
	    f[i].x = x;
	    f[i].y = y+64;
	    f[i].next = &f[i+1];f[i++].type = gfx_lineTo;
	    f[i].x = x;
	    f[i].y = y;
	    f[i].next = &f[i+1];f[i++].type = gfx_lineTo;
	}
	f[i-1].next = 0;

	double width = 40;
	
	//gfxdrawer_t d;
	//gfxdrawer_target_gfxline(&d);
	//draw_stroke(l, &d, width, gfx_capRound, gfx_joinBevel, 500);
	//gfxline_t*line = (gfxline_t*)d.result(&d);

	//gfxpoly_t*p = gfxpoly_fromstroke(l, width, gfx_capRound, gfx_joinRound, 500);
	gfxpoly_t*p1 = gfxpoly_from_stroke(l, width, gfx_capRound, gfx_joinRound, 500, 0.05);
	assert(gfxpoly_check(p1));
	
	//gfxpoly_t*p2 = gfxpoly_from_fill(f, 0.05);
	gfxline_t*l2 = gfxline_clone(l);

	double c = cos(t*M_PI/75);
	double s = sin(t*M_PI/75);
	static int x1 = 0, xdir = 1;
	static int y1 = 0, ydir = 5;
	x1+=xdir; if(x1>=150)  {x1=300-x1;xdir=-xdir;} if(x1<-150) {x1=-300-x1;xdir=-xdir;}
	y1+=ydir; if(y1>=150)  {y1=300-y1;ydir=-ydir;} if(y1<-150) {y1=-300-y1;ydir=-ydir;}
	gfxmatrix_t m = { c, s, -(350+x1)*c-350*s+350,
	                 -s, c,  350*s-(350+x1)*c+350};
	gfxline_transform(l2, &m);
	gfxpoly_t*p2 = gfxpoly_from_stroke(l2, width, gfx_capRound, gfx_joinRound, 500, 0.05);
	assert(gfxpoly_check(p2));

	gfxpoly_t*p3 = gfxpoly_intersect(p1, p2);
	assert(gfxpoly_check(p3));
	
	//gfxpoly_t*p4 = gfxpoly_from_fill(f, 0.05);
	//gfxpoly_t*p5 = gfxpoly_intersect(p1, p4);

	gfxline_t*line = gfxline_from_gfxpoly(p3);
	gfxpoly_destroy(p1);
	gfxpoly_destroy(p2);
	gfxpoly_destroy(p3);

	//gfxline_dump(line, stdout, "");

	gfxcolor_t blue = {255,0,0,255};
	gfxline_t*r = gfxline_makerectangle(0,0,700,700);
	dev.fill(&dev, r, &blue);

	gfxcolor_t black = {255,0,0,64};
	gfxcolor_t white = {255,255,255,255};
	gfxcolor_t cyan = {255,0,192,192};
	//dev.stroke(&dev, l, 2, &black, gfx_capRound, gfx_joinRound, 0);
	//dev.stroke(&dev, line, 2, &cyan, gfx_capRound, gfx_joinRound, 0);
	dev.fill(&dev, line, &black);
	gfxmatrix_t m2 = {1.0,   0, -10,
	                   0, 1.0, -10};
	gfxline_transform(line, &m2);
	dev.fill(&dev, line, &cyan);
	dev.stroke(&dev, line, 2.0, &white, gfx_capRound, gfx_joinRound, 0);

	gfxline_free(line);
	dev.endpage(&dev);
    }

    gfxresult_t* result = dev.finish(&dev);
    result->save(result, "test.swf");
    result->destroy(result);
}
