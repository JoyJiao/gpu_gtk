#include<gtk/gtk.h>
#include<gdk/gdk.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>

static GdkPixmap *pixmap = NULL;
int radius;
int gintnpoints = 0;
GdkPoint points[200];
char buffer[50];
void DrawTickAt (GdkDrawable *pixmap, GdkGC *gc, int nHour, int cx, int cy)
{
  
    double dRadians = nHour * 3.14 / 6.0;
  
    gdk_draw_line (pixmap, gc,
                   cx+(int) ((0.95 * radius * sin (dRadians))),
                   cy-(int) ((0.95 * radius * cos (dRadians))),
                   cx+(int) ((1.0 * radius * sin (dRadians))),
                   cy-(int) ((1.0 * radius * cos (dRadians))));
}

gint Repaint(gpointer data)
{
    GtkWidget*    drawing_area = (GtkWidget *) data;
    GdkRectangle  update_rect;
    int midx, midy;
    int nHour;
    float dRadians;
    time_t now;
    struct tm *now_tm;
  
   gdk_draw_rectangle (pixmap,
              drawing_area->style->white_gc,
              TRUE,
              0, 0,
              drawing_area->allocation.width,
              drawing_area->allocation.height);
  
    midx = drawing_area->allocation.width / 2;
    midy = drawing_area->allocation.height / 2;
  
    radius = MIN (midx, midy);
  
   /* gdk_draw_arc (pixmap,
                  drawing_area->style->black_gc,
                  0,
                  0, 0, midx + midx, midy + midy, 0, 360 * 64);
  
    for (nHour = 1; nHour <= 12; nHour++) {
        DrawTickAt (pixmap, drawing_area->style->black_gc,
                    nHour, midx, midy);
    }*/
  
    time (&now);
    now_tm = localtime (&now);
  

    GdkGC *gc1, *gc2, *gc;  /*gc1为矩形，gc2为文字*/
    GdkColor color;
    gc1 = gdk_gc_new( drawing_area->window );
    color.red = 65535;
    color.green = 0;
    color.blue = 65535;
    gdk_gc_set_rgb_fg_color(gc1, &color );
   
    gdk_draw_lines(pixmap,
    gc1,
    points,
    gintnpoints++);
   

    sprintf(buffer,"heart_rate: %d",gintnpoints);/*赋予数值*/
   
    PangoLayout *layout =
        gtk_widget_create_pango_layout( drawing_area, buffer );
    PangoFontDescription *fontdesc =
        pango_font_description_from_string( "Luxi Mono 12" );
    pango_layout_set_font_description( layout, fontdesc );
    gdk_draw_layout( pixmap,
                     gc1,
                     5, 5, layout );
    if(gintnpoints == 100)
        gintnpoints = 0;
   
    printf("%d\n",gintnpoints);
    dRadians = now_tm->tm_sec * 3.14 / 30.0;
  
  /*  gdk_draw_line (pixmap, drawing_area->style->black_gc,
                   midx, midy,
                   midx + (0.9 * radius * sin (dRadians)),
                   midy - (0.9 * radius * cos (dRadians)));
  
  
    dRadians = (now_tm->tm_min * 3.14 / 30.0) +
               (3.14 * now_tm->tm_sec / 1800.0);
  
    gdk_draw_line (pixmap, drawing_area->style->black_gc,
                   midx, midy,
                   midx+(int) (0.7 * radius * sin (dRadians)),
                   midy-(int) (0.7 * radius * cos (dRadians)));
  
  
    dRadians = (now_tm->tm_hour % 12) * 3.14 / 6.0 +
               (3.14 * now_tm->tm_min / 360.0);
  
    gdk_draw_line (pixmap, drawing_area->style->black_gc,
                   midx, midy,
                   midx + (int) (radius * 0.5 * sin (dRadians)),
                   midy - (int) (radius * 0.5 * cos (dRadians)));*/

    update_rect.x = 0;
    update_rect.y = 0;
    update_rect.width = drawing_area->allocation.width*3/4;
    update_rect.height = drawing_area->allocation.height*3/4;
  
    gtk_widget_draw (drawing_area, &update_rect);
}

static gint configure_event (GtkWidget *widget, GdkEventConfigure *event)
{
  
    if (pixmap) {
        gdk_pixmap_unref (pixmap);
    }
  
    pixmap = gdk_pixmap_new (widget->window,
                widget->allocation.width,
                widget->allocation.height,
                -1);
    return TRUE;
}

gint expose_event (GtkWidget *widget, GdkEventExpose *event)
{
  
    gdk_draw_pixmap (widget->window,
          widget->style->fg_gc[GTK_WIDGET_STATE (widget)],
          pixmap,
          event->area.x, event->area.y,
          event->area.x, event->area.y,
          event->area.width, event->area.height);
    return FALSE;
}

void quit ()
{
    gtk_exit (0);
}

//int main (int argc, char *argv[])
void main (void *arg)
{

    GtkWidget *button;
    GtkWidget *halign;
    int argc=0; 
    char **argv=NULL;

    int i;
    for( i = 0;i<200;i++) {
        points[i].x = i*5;
        points[i].y = (int)100*sin(i)+200;
    } 
   
     printf("@@@%d,,%d,,%d,,%d\n",points[0].x, points[0].y,
                   points[100].x, points[100].y);
    GtkWidget *window;
    GtkWidget *drawing_area;
    GtkWidget *vbox;
   gtk_init (&argc, &argv);
     
    window = gtk_window_new (GTK_WINDOW_POPUP);
    vbox = gtk_hbox_new (FALSE, 0);
    gtk_container_add (GTK_CONTAINER (window), vbox);
    gtk_widget_show (vbox);
    gtk_signal_connect (GTK_OBJECT (window), "destroy",
                GTK_SIGNAL_FUNC (quit), NULL);
  //
 //申请据边界的一块区域
 /* gtk_container_set_border_width(GTK_CONTAINER(window),200);

 //链接
  button = gtk_button_new_with_mnemonic("_Button"); 
  g_signal_connect(button, "clicked", 
      G_CALLBACK(Repaint), NULL); 

 
 //添加标签
 // button = gtk_button_new_with_label("Button");
  gtk_widget_set_tooltip_text(button, "Button widget");

  //坐标（x,y,,x_scale,y_acle）
  halign = gtk_alignment_new(0, 0, 0, 0);
  gtk_container_add(GTK_CONTAINER(halign), button);
  gtk_container_add(GTK_CONTAINER(window), halign); 

  //
*/

    drawing_area = gtk_drawing_area_new ();
    gtk_drawing_area_size (GTK_DRAWING_AREA (drawing_area), 500, 500);
    gtk_box_pack_start (GTK_BOX (vbox), drawing_area, TRUE, TRUE, 0);
    gtk_widget_show (drawing_area);
  
    gtk_signal_connect (GTK_OBJECT (drawing_area), "expose_event",
                (GtkSignalFunc) expose_event, NULL);
    gtk_signal_connect (GTK_OBJECT(drawing_area),"configure_event",
                (GtkSignalFunc) configure_event, NULL);
  
    gtk_widget_show (window);
  
    gtk_timeout_add (100, Repaint, drawing_area);

    gtk_main ();
   // return 0;
}
