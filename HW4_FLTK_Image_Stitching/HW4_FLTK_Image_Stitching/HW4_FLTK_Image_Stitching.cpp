#include "pch.h"
#include <iostream>
#include <opencv2/opencv.hpp>
#include <FL/Fl.h>
#include <FL/Fl_Box.h>
#include <FL/Fl_Window.h>
#include <FL/Fl_Button.H>
#include "source_view_tmp.h"
#include <FL/Fl_JPEG_Image.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_BMP_Image.H>
#include <FL/Fl_File_Chooser.H>
#include <FL/Fl_Text_Buffer.H>
#include <FL/Fl_Text_Display.H>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <FL/Fl.H>
#include <FL/fl_draw.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Native_File_Chooser.h>
#include <FL/Fl_Output.H>
#include <FL/Fl_Menu_Bar.H>
#include <FL/Fl_Value_Slider.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Text_Display.H>
#include <Fl/fl_ask.h>
#include <Fl/fl_file_chooser.h>
using namespace std;
using namespace cv;

const char* currentFilename = "./white.bmp";
Fl_File_Chooser fc("./image_choose", "BMP (*.bmp)", Fl_File_Chooser::MULTI, "File Chooser");
Fl_Box* img_ori_box_ori1;
Fl_Box* img_ori_box_ori2;
Fl_Box* img_ori_box_ori3;
Fl_Box* img_ori_box_ori4;
Fl_Box* img_ori_box_cut1;
Fl_Box* img_ori_box_cut2;
Fl_Box* img_ori_box_cut3;
Fl_Box* img_ori_box_cut4;
Fl_Box* img_merge_box;
Fl_Text_Buffer* buffer;
Fl_Text_Display* display;
Fl_Value_Slider* sld_img12;
Fl_Value_Slider* sld_img23;
Fl_Value_Slider* sld_img34;
Mat img_ori1;
Mat img_ori2;
Mat img_ori3;
Mat img_ori4;
Mat merge_img;
Mat img1;
Mat img2;
Mat img3;
Mat img4;
string str1;
string str2;
string str3;
string str4;
string buffer_text;
bool try_use_gpu = false;
bool img_box1 = false;
bool img_box2 = false;
bool img_box3 = false;
bool img_box4 = false;
double overlap = 0.8;
vector<Mat> imgs;
Fl_Menu_Bar* menu_bar;
vector <Rect> tmp_rect;
vector<vector<Rect>> rois;
double overlap12 = 0;
double overlap23 = 0;
double overlap34 = 0;

Mat image_merge(string img1_filename, string img2_filename, string img3_filename, string img4_filename);
void cb_about();
void btn_load_cb(Fl_Widget* w, void* p);
void btn_merge_cb(Fl_Widget* w, void* p);
void btn_reset_cb(Fl_Widget* w, void* p);
void FileChooser_cb(Fl_File_Chooser* fc, void* data);
void gui_box_reset(Fl_Box* img_ori_box_ori1, Fl_Box* img_ori_box_ori2, Fl_Box* img_ori_box_ori3, Fl_Box* img_ori_box_ori4, Fl_Box* img_merge_box);
void btn_save_cb(Fl_Widget* w);
void btn_quit_cb(Fl_Widget* w);
void cut_image(string img1_filename, string img2_filename, string img3_filename, string img4_filename);
void img1_cut(int img_num);
void img12_cut(int img_num);
void img23_cut(int img_num);
void img32_cut(int img_num);
void img34_cut(int img_num);
void img43_cut(int img_num);
void btn_cut_cb(Fl_Widget* w);
void FileChooser_save_img_cb(Fl_File_Chooser* w, void* userdata);
void sld_img12_cb(Fl_Widget* w);
void sld_img23_cb(Fl_Widget* w);
void sld_img34_cb(Fl_Widget* w);

// Fl_Menu_Item為FLTK函式庫，目的建立使用者界面中最上面選單
Fl_Menu_Item menuitems[] = {
    {"&File", 0, 0, 0, FL_SUBMENU},
    {"File_&Selector...", FL_CTRL + 'f',(Fl_Callback*)btn_load_cb},
    {"&Image_cut", FL_CTRL + 'c',(Fl_Callback*)btn_cut_cb},
    {"&Reset", FL_CTRL + 'r',btn_reset_cb},
    {"&Image_merge", FL_CTRL + 'm',(Fl_Callback*)btn_merge_cb},
    {"File_&Save", FL_CTRL + 's',(Fl_Callback*)btn_save_cb},
    {"&Quit", FL_CTRL + 'q',(Fl_Callback*)btn_quit_cb},
    {0},
    {"&Help", 0, 0, 0, FL_SUBMENU},
    {"&About", FL_CTRL + 'a', (Fl_Callback*)cb_about},
    {0},
    {0}
};

/* 使用者介面最上面有按鈕是 about */
void cb_about()
{
    fl_message("HW4_FLTK_Image_Stitching_609410151.");
}

/* 調用 FileChooser_cb 函數 */
void btn_load_cb(Fl_Widget* w, void* p)
{
    fc.callback(FileChooser_cb);
    fc.show();
}

/* 對圖片進行合併 */
void btn_merge_cb(Fl_Widget* w, void* p)
{
    // 沒有用處
    Fl_Button* o1 = (Fl_Button*)w;
    // 要將合併完的圖片放到哪個 box 內
    Fl_Box* o2 = (Fl_Box*)img_merge_box;
    // 存儲合併完的圖片
    Mat result_img;

    // 計算合併圖片所需時間的起始
    double merge_time_start = getTickCount();
    // 進行圖片合併處理
    result_img = image_merge(str1, str2, str3, str4);
    // 存儲合併結果
    imwrite("result.bmp", result_img);

    // 將合併結果更新到 box 上面
    currentFilename = "result.bmp";
    Fl_BMP_Image* bmp_img = new Fl_BMP_Image(currentFilename);
    Fl_Image* bmp_img2 = bmp_img->copy(1000, 400);
    o2->image(bmp_img2);
    o2->redraw();

    // 更新文字提示框
    buffer->append("Merge picture completed\n");
    double merge_time_total = ((double)getTickCount() - merge_time_start) / getTickFrequency();
    buffer->append("Time:");
    string merge_time_total_string = to_string(merge_time_total);
    const char* merge_time_total_char = merge_time_total_string.c_str();
    buffer->append(merge_time_total_char);
    buffer->append("\n\n");
    display->buffer(buffer);
}

/* 重置 */
void btn_reset_cb(Fl_Widget* w, void* p)
{
    // 更新文字顯示框
    buffer->append("Program reset\n");
    // 計算重置起始時間
    double reset_time_start = getTickCount();
    // 進行 box 重置
    gui_box_reset(img_ori_box_ori1, img_ori_box_ori2, img_ori_box_ori3, img_ori_box_ori4, img_merge_box);
    currentFilename = "./white.bmp";
    // 資源重置
    img_ori1.release();
    img_ori2.release();
    img_ori3.release();
    img_ori4.release();
    merge_img.release();

    str1.clear();
    str2.clear();
    str3.clear();
    str4.clear();
    img_box1 = false;
    img_box2 = false;
    img_box3 = false;
    img_box4 = false;

    imgs.clear();
    tmp_rect.clear();
    rois.clear();
    // 更新文字提示框內容顯示重置時間
    double reset_time_total = ((double)getTickCount() - reset_time_start) / getTickFrequency();
    buffer->append("Time:");
    string reset_time_total_string = to_string(reset_time_total);
    const char* reset_time_total_char = reset_time_total_string.c_str();
    buffer->append(reset_time_total_char);
    buffer->append("\n\n");
    buffer->append("********************************\n");
    buffer->append("\n\n");
    display->buffer(buffer);
}

/* 實現選擇檔案功能 */
void FileChooser_cb(Fl_File_Chooser* fc, void* data)
{
    // 獲得從選擇器中選擇的文件名列表
    for (int i = 1; i <= fc->count(); i++)
    {
        currentFilename = fc->value(i);
    }

    // 對選擇的路徑讀取圖片
    Fl_BMP_Image* bmp_img_btn = new Fl_BMP_Image(currentFilename);
    // 暫存一張圖片
    Fl_Image* bmp_img_btn2 = bmp_img_btn->copy(400, 200);

    /* 接下來需要按照順序對每個 box 進行順序讀取 */
    // 如果第一個 box 為空就先讀入第一個 box，依次類推
    if (!img_box1)
    {
        // 因為需要計算每個圖片讀取時間，先建立一個 img1 讀取的起始時間
        double load_img1_time_start = getTickCount();
        // 讀取圖片
        img_ori1 = imread(currentFilename);
        // 記錄第一張圖片讀取的路徑，其他函數會使用
        str1 = currentFilename;
        // 更新文字提示框內容，更新讀取多少時間
        buffer->append("Loading image 1\n");
        double load_img1_time_total = ((double)getTickCount() - load_img1_time_start) / getTickFrequency();
        buffer->append("Time:");
        string load_img1_time_total_string = to_string(load_img1_time_total);
        const char* load_img1_time_total_char = load_img1_time_total_string.c_str();
        buffer->append(load_img1_time_total_char);
        buffer->append("\n\n");
        display->buffer(buffer);
        // 更新目的為img1已經讀取圖片
        img_box1 = true;
        // 將圖片更新到 box1 上
        img_ori_box_ori1->image(bmp_img_btn2);
        img_ori_box_ori1->redraw();
    }
    else if (!img_box2)
    {
        double load_img2_time_start = getTickCount();
        img_ori2 = imread(currentFilename);
        str2 = currentFilename;
        buffer->append("Loading image 2\n");
        double load_img2_time_total = ((double)getTickCount() - load_img2_time_start) / getTickFrequency();
        buffer->append("Time:");
        string load_img2_time_total_string = to_string(load_img2_time_total);
        const char* load_img2_time_total_char = load_img2_time_total_string.c_str();
        buffer->append(load_img2_time_total_char);
        buffer->append("\n\n");
        display->buffer(buffer);
        img_box2 = true;
        img_ori_box_ori2->image(bmp_img_btn2);
        img_ori_box_ori2->redraw();
    }
    else if (!img_box3)
    {
        double load_img3_time_start = getTickCount();
        img_ori3 = imread(currentFilename);
        str3 = currentFilename;
        buffer->append("Loading image 3\n");
        double load_img3_time_total = ((double)getTickCount() - load_img3_time_start) / getTickFrequency();
        buffer->append("Time:");
        string load_img3_time_total_string = to_string(load_img3_time_total);
        const char* load_img3_time_total_char = load_img3_time_total_string.c_str();
        buffer->append(load_img3_time_total_char);
        buffer->append("\n\n");
        display->buffer(buffer);
        img_box3 = true;
        img_ori_box_ori3->image(bmp_img_btn2);
        img_ori_box_ori3->redraw();
    }
    else if (!img_box4)
    {
        double load_img4_time_start = getTickCount();
        img_ori4 = imread(currentFilename);
        str4 = currentFilename;
        buffer->append("Loading image 4\n");
        double load_img4_time_total = ((double)getTickCount() - load_img4_time_start) / getTickFrequency();
        buffer->append("Time:");
        string load_img4_time_total_string = to_string(load_img4_time_total);
        const char* load_img4_time_total_char = load_img4_time_total_string.c_str();
        buffer->append(load_img4_time_total_char);
        buffer->append("\n\n");
        display->buffer(buffer);
        img_box4 = true;
        img_ori_box_ori4->image(bmp_img_btn2);
        img_ori_box_ori4->redraw();
    }
}

/* 圖片合併 */
Mat image_merge(string img1_filename, string img2_filename, string img3_filename, string img4_filename)
{
    // 創建一個拼接容器
    Stitcher stitcher = Stitcher::createDefault(try_use_gpu);

    // 在img43_cut等函式中將圖片放入imgs
    // 在img43_cut等函式中將圖片放入rois
    Stitcher::Status status = stitcher.stitch(imgs, rois, merge_img);
    //stitcher.setFeaturesFinder(new detail::SiftFeaturesFinder());

    if (status != Stitcher::OK)
    {
        cout << "Can't stitch images, error code = " << int(status) << endl;
    }
    //imwrite(result_name, merge_img);

    return merge_img;
}

/* 對 box 進行重置 */
void gui_box_reset(Fl_Box* img_ori_box_ori1, Fl_Box* img_ori_box_ori2, Fl_Box* img_ori_box_ori3, Fl_Box* img_ori_box_ori4, Fl_Box* img_merge_box)
{
    // 將全白圖進行覆蓋
    Fl_BMP_Image* bmp_img = new Fl_BMP_Image("white.bmp");
    Fl_Image* bmp_img2 = bmp_img->copy(390, 200);
    img_ori_box_ori1->image(bmp_img2);
    img_ori_box_ori1->redraw();
    img_ori_box_ori2->image(bmp_img2);
    img_ori_box_ori2->redraw();
    img_ori_box_ori3->image(bmp_img2);
    img_ori_box_ori3->redraw();
    img_ori_box_ori4->image(bmp_img2);
    img_ori_box_ori4->redraw();
    img_ori_box_cut1->image(bmp_img2);
    img_ori_box_cut1->redraw();
    img_ori_box_cut2->image(bmp_img2);
    img_ori_box_cut2->redraw();
    img_ori_box_cut3->image(bmp_img2);
    img_ori_box_cut3->redraw();
    img_ori_box_cut4->image(bmp_img2);
    img_ori_box_cut4->redraw();
    bmp_img2 = bmp_img->copy(1300, 400);
    img_merge_box->image(bmp_img2);
    img_merge_box->redraw();
}

/* 將結果進行存儲到指定路徑 */
void btn_save_cb(Fl_Widget* w)
{
    Fl_File_Chooser* fc_save_img = new Fl_File_Chooser("./save_img/", "/", Fl_File_Chooser::DIRECTORY, "File Chooser save image");
    const char* save_file_name;
    fc_save_img->callback(FileChooser_save_img_cb);
    fc_save_img->show();
}

/* 離開程式 */
void btn_quit_cb(Fl_Widget* w)
{
    buffer->append("Quit\n");
    double quit_time_start = getTickCount();

    double quit_time_total = ((double)getTickCount() - quit_time_start) / getTickFrequency();
    buffer->append("Time:");
    string quit_time_total_string = to_string(quit_time_total);
    const char* quit_time_total_char = quit_time_total_string.c_str();
    buffer->append(quit_time_total_char);
    buffer->append("\n\n");
    display->buffer(buffer);

    exit(1);
}

/* 對選擇圖片進行切割 */
void cut_image(string img1_filename, string img2_filename, string img3_filename, string img4_filename)
{
    // 更新文字提示框與記錄切割所需初始時間
    buffer->append("Image cut\n");
    double cut_time_start = getTickCount();
    // 讀取圖片
    img1 = imread(img1_filename);
    img2 = imread(img2_filename);
    img3 = imread(img3_filename);
    img4 = imread(img4_filename);
    // 沒有用處
    int img_num = 0;
    cout << img1.cols << endl;
    // 確定讀取多少圖片
    // 有四張圖片
    if (!img1.empty() && !img2.empty() && !img3.empty() && !img4.empty())
    {
        int img_num = 4;
        // 能存放多少元素
        rois.resize(img_num);
        // 對第一張圖右邊進行切割
        img1_cut(img_num);
        // 對第二張圖左邊進行切割
        img12_cut(img_num);
        // 對第二張圖右邊進行切割
        img23_cut(img_num);
        // 對第三張圖左邊進行切割
        img32_cut(img_num);
        // 對第三張圖右邊進行切割
        img34_cut(img_num);
        // 對第四張圖左邊進行切割
        img43_cut(img_num);
    }
    else if (!img1.empty() && !img2.empty() && !img3.empty())
    {
        int img_num = 3;
        rois.resize(img_num);
        img1_cut(img_num);
        img12_cut(img_num);
        img23_cut(img_num);
        img32_cut(img_num);
    }
    else if (!img1.empty() && !img2.empty())
    {
        int img_num = 2;
        rois.resize(img_num);
        img1_cut(img_num);
        img12_cut(img_num);
    }

    // 計算切割時間並更新
    double cut_time_total = ((double)getTickCount() - cut_time_start) / getTickFrequency();
    buffer->append("Time:");
    string cut_time_total_string = to_string(cut_time_total);
    const char* cut_time_total_char = cut_time_total_string.c_str();
    buffer->append(cut_time_total_char);
    buffer->append("\n\n");
    display->buffer(buffer);
}

void img43_cut(int img_num)
{
    int x_coordinate;

    // Image 4 操作
    imgs.push_back(img4);

    //x坐標做overlap
    x_coordinate = round((double)(imgs[3].cols) * overlap34);
    x_coordinate = imgs[3].cols - x_coordinate;

    //選擇rect
    Rect rect43(0, 0, imgs[3].cols - x_coordinate, imgs[3].rows);
    Mat roi43;
    Mat img43_tmp;
    img4(rect43).copyTo(roi43);
    img4.copyTo(img43_tmp);
    tmp_rect.push_back(rect43);
    rois[3] = (tmp_rect);
    tmp_rect.clear();
    rectangle(img43_tmp, rect43, Scalar(255, 0, 0), 8);
    imwrite("./img_tmp/img43_draw.bmp", img43_tmp);
    string s_tmp43 = "./img_tmp/img43_draw.bmp";
    const char* img_tmp_char43 = s_tmp43.c_str();
    Fl_BMP_Image* bmp_img_tmp43 = new Fl_BMP_Image(img_tmp_char43);
    Fl_Image* img_tmp43 = bmp_img_tmp43->copy(390, 200);
    imwrite("./img_tmp/img43_cp.bmp", roi43);
    img_ori_box_cut4->image(img_tmp43);
    img_ori_box_cut4->redraw();
}

void img34_cut(int img_num)
{
    int x_coordinate;

    // Image 3 操作
    //imgs.push_back(img3);

    //x坐標做overlap
    x_coordinate = round((double)(imgs[2].cols) * overlap34);
    x_coordinate = imgs[2].cols - x_coordinate;

    //選擇rect
    Rect rect34(x_coordinate, 0, imgs[2].cols - x_coordinate, imgs[2].rows);
    Mat roi34;
    Mat img34_tmp;
    img3(rect34).copyTo(roi34);
    img3.copyTo(img34_tmp);
    tmp_rect.push_back(rect34);
    rois[2] = (tmp_rect);
    tmp_rect.clear();
    Mat img23_tmp = imread("./img_tmp/img32_draw.bmp");
    rectangle(img23_tmp, rect34, Scalar(255, 0, 0), 8);
    imwrite("./img_tmp/img34_draw.bmp", img23_tmp);
    string s_tmp34 = "./img_tmp/img34_draw.bmp";
    const char* img_tmp_char34 = s_tmp34.c_str();
    Fl_BMP_Image* bmp_img_tmp34 = new Fl_BMP_Image(img_tmp_char34);
    Fl_Image* img_tmp34 = bmp_img_tmp34->copy(390, 200);
    imwrite("./img_tmp/img34_cp.bmp", roi34);
    img_ori_box_cut3->image(img_tmp34);
    img_ori_box_cut3->redraw();
}

void img32_cut(int img_num)
{
    int x_coordinate;

    // Image 3 操作
    imgs.push_back(img3);

    //x坐標做overlap
    x_coordinate = round((double)(imgs[2].cols) * overlap23);
    x_coordinate = imgs[2].cols - x_coordinate;

    //選擇rect
    Rect rect32(0, 0, imgs[2].cols - x_coordinate, imgs[2].rows);
    Mat roi32;
    Mat img32_tmp;
    img3(rect32).copyTo(roi32);
    img3.copyTo(img32_tmp);
    tmp_rect.push_back(rect32);
    rois[2] = (tmp_rect);
    tmp_rect.clear();
    rectangle(img32_tmp, rect32, Scalar(0, 0, 255), 8);
    imwrite("./img_tmp/img32_draw.bmp", img32_tmp);
    string s_tmp32 = "./img_tmp/img32_draw.bmp";
    const char* img_tmp_char32 = s_tmp32.c_str();
    Fl_BMP_Image* bmp_img_tmp32 = new Fl_BMP_Image(img_tmp_char32);
    Fl_Image* img_tmp32 = bmp_img_tmp32->copy(390, 200);
    imwrite("./img_tmp/img32_cp.bmp", roi32);
    img_ori_box_cut3->image(img_tmp32);
    img_ori_box_cut3->redraw();
}

void img23_cut(int img_num)
{
    int x_coordinate;

    // Image 2 操作
    //imgs.push_back(img2);

    //x坐標做overlap
    x_coordinate = round((double)(imgs[1].cols) * overlap23);
    x_coordinate = imgs[1].cols - x_coordinate;

    //選擇rect
    Rect rect23(x_coordinate, 0, imgs[1].cols - x_coordinate, imgs[1].rows);
    Mat roi23;
    Mat img23_tmp;
    img2(rect23).copyTo(roi23);
    img2.copyTo(img23_tmp);
    tmp_rect.push_back(rect23);
    rois[1] = (tmp_rect);
    tmp_rect.clear();
    Mat img12_tmp = imread("./img_tmp/img12_draw.bmp");
    rectangle(img12_tmp, rect23, Scalar(0, 0, 255), 8);
    imwrite("./img_tmp/img23_draw.bmp", img12_tmp);
    string s_tmp23 = "./img_tmp/img23_draw.bmp";
    const char* img_tmp_char23 = s_tmp23.c_str();
    Fl_BMP_Image* bmp_img_tmp23 = new Fl_BMP_Image(img_tmp_char23);
    Fl_Image* img_tmp23 = bmp_img_tmp23->copy(390, 200);
    imwrite("./img_tmp/img23_cp.bmp", roi23);
    img_ori_box_cut2->image(img_tmp23);
    img_ori_box_cut2->redraw();
}

void img12_cut(int img_num)
{
    int x_coordinate;

    // Image 2 操作
    imgs.push_back(img2);

    //x坐標做overlap
    x_coordinate = round((double)(imgs[1].cols) * overlap12);
    x_coordinate = imgs[1].cols - x_coordinate;

    //選擇rect
    Rect rect12(0, 0, imgs[1].cols - x_coordinate, imgs[1].rows);
    Mat roi12;
    Mat img12_tmp;
    img2(rect12).copyTo(roi12);
    img2.copyTo(img12_tmp);
    // tmp_rect用來存放
    tmp_rect.push_back(rect12);
    rois[1] = (tmp_rect);
    tmp_rect.clear();
    rectangle(img12_tmp, rect12, Scalar(0, 255, 255), 8);
    imwrite("./img_tmp/img12_draw.bmp", img12_tmp);
    string s_tmp12 = "./img_tmp/img12_draw.bmp";
    const char* img_tmp_char12 = s_tmp12.c_str();
    Fl_BMP_Image* bmp_img_tmp12 = new Fl_BMP_Image(img_tmp_char12);
    Fl_Image* img_tmp12 = bmp_img_tmp12->copy(390, 200);
    imwrite("./img_tmp/img12_cp.bmp", roi12);
    img_ori_box_cut2->image(img_tmp12);
    img_ori_box_cut2->redraw();
}

/* 對第一張圖右邊進行切割 */
void img1_cut(int img_num)
{
    int x_coordinate = 0;
    // 將img1 然如 vector 中
    imgs.push_back(img1);

    // 計算切割的 x 起點
    // 計算方式：column的總長乘上需要多少比例去剪掉column的總長
    x_coordinate = round((double)(imgs[0].cols) * overlap12);
    // 全部剪掉需要比例 = x，後面再全部剪掉 x ，等於需要比例
    // ex：全部100，要60%，現在x=40，往下五行會將全部在剪掉x，得到60
    x_coordinate = imgs[0].cols - x_coordinate;

    // 計算分割的大小 rect 坐標 (最左上 , 最右下)
    Rect rect1(x_coordinate, 0, imgs[0].cols - x_coordinate, imgs[0].rows);
    Mat roi1;
    Mat img1_tmp;
    // 在img1下使用rect1的坐標進行裁切
    // 將裁剪完的結果放入roi1
    img1(rect1).copyTo(roi1);
    // 暫存一張圖用來將切割部分長方形畫在上面
    img1.copyTo(img1_tmp);
    // 將裁切的長方形坐標放入tmp_rect
    tmp_rect.push_back(rect1);
    // rois用來存儲切割全部坐標，合併時需要這個參數來合併
    rois[0] = (tmp_rect);
    tmp_rect.clear();
    // 在box畫出所需要的長方形，表示切割部分
    rectangle(img1_tmp, rect1, Scalar(0, 255, 255), 8);
    imwrite("./img_tmp/img1_draw.bmp", img1_tmp);
    string s_tmp1 = "./img_tmp/img1_draw.bmp";
    const char* img_tmp_char = s_tmp1.c_str();
    Fl_BMP_Image* bmp_img_tmp = new Fl_BMP_Image(img_tmp_char);
    Fl_Image* bmp_img_tmp2 = bmp_img_tmp->copy(390, 200);
    imwrite("./img_tmp/img1_cp.bmp", roi1);
    img_ori_box_cut1->image(bmp_img_tmp2);
    img_ori_box_cut1->redraw();
}

/* 對圖片進行選擇比例切割 */
void btn_cut_cb(Fl_Widget* w)
{
    cut_image(str1, str2, str3, str4);
}

/* 存儲圖片 */
void FileChooser_save_img_cb(Fl_File_Chooser* w, void* userdata)
{
    const char* save_file_name = w->value();
    string save_img_name_string = save_file_name;
    save_img_name_string = save_img_name_string + "save_img.bmp";
    buffer->append("Image save\n");
    double save_img_time_start = getTickCount();
    imwrite(save_img_name_string, merge_img);

    double save_img_time_total = ((double)getTickCount() - save_img_time_start) / getTickFrequency();
    buffer->append("Time:");
    string save_img_time_total_string = to_string(save_img_time_total);
    const char* save_img_time_total_char = save_img_time_total_string.c_str();
    buffer->append(save_img_time_total_char);
    buffer->append("\n\n");
    display->buffer(buffer);
}

/* 調整滑動條對 img1 與 img2 所重疊的比例 */
void sld_img12_cb(Fl_Widget* w)
{
    int tmp = (int)sld_img12->value();
    overlap12 = tmp / 100.0;
}

/* 調整滑動條對 img2 與 img3 所重疊的比例 */
void sld_img23_cb(Fl_Widget* w)
{
    int tmp = (int)sld_img23->value();
    overlap23 = tmp / 100.0;
}

/* 調整滑動條對 img3 與 img4 所重疊的比例 */
void sld_img34_cb(Fl_Widget* w)
{
    int tmp = (int)sld_img34->value();
    overlap34 = tmp / 100.0;
}

int main()
{
    /* 建立 window */
    Fl_Double_Window* window = new Fl_Double_Window(1850, 1000, "HW4_609410151");
    window->begin();


    /* 建立 window 最上面的 menu 選單 */
    menu_bar = new Fl_Menu_Bar(0, 0, 1850, 25);
    menu_bar->menu(menuitems);


    /* 建立 button */
    Fl_Button* btn_load = new Fl_Button(55, 525, 100, 30, "Load");
    Fl_Button* btn_cut = new Fl_Button(55, 580, 100, 30, "Cut");
    Fl_Button* btn_merge = new Fl_Button(55, 640, 100, 30, "Merge");
    Fl_Button* btn_reset = new Fl_Button(55, 700, 100, 30, "Reset");
    Fl_Button* btn_save = new Fl_Button(55, 755, 100, 30, "Save");
    Fl_Button* btn_quit = new Fl_Button(55, 805, 100, 30, "Quit");


    /* 建立 文字提示框 */
    buffer = new Fl_Text_Buffer();
    display = new Fl_Text_Display(15, 40, 200, 465);
    // 並在文字提示框中初始化內容
    buffer->append("********************************\n");
    buffer->append("The program is created\n");
    buffer->append("********************************\n");
    display->buffer(buffer);


    /* 建立四個 box 用來顯示讀取的原始圖片 */
    img_ori_box_ori1 = new Fl_Box(240, -60, 390, 390 /*, "img_ori_1"*/);
    img_ori_box_ori2 = new Fl_Box(640, -60, 390, 390/*, "img_ori_2" */);
    img_ori_box_ori3 = new Fl_Box(1040, -60, 390, 390/*, "img_ori_3" */);
    img_ori_box_ori4 = new Fl_Box(1440, -60, 390, 390/*, "img_ori_4" */);


    /* 建立 滑動條 用來調整不同圖片之間重疊比例 */
    // sld_img12 為第一與第二張圖片
    sld_img12 = new Fl_Value_Slider(530, 270, 200, 30, "image1 & image2");
    // 設定最大值與最小值
    sld_img12->bounds(0, 100);
    // 建立水平形態
    sld_img12->type(FL_HOR_NICE_SLIDER);
    // sld_img23 為第二與第三張圖片
    sld_img23 = new Fl_Value_Slider(930, 270, 200, 30, "image2 & image3");
    sld_img23->bounds(0, 100);
    sld_img23->type(FL_HOR_NICE_SLIDER);
    // sld_img34 為第三與第四張圖片
    sld_img34 = new Fl_Value_Slider(1330, 270, 200, 30, "image3 & image4");
    sld_img34->bounds(0, 100);
    sld_img34->type(FL_HOR_NICE_SLIDER);

    /* 建立四個 box 用來顯示讀取的原始圖片進行分割比例 */
    img_ori_box_cut1 = new Fl_Box(240, 240, 390, 390 /*, "img_ori_1"*/);
    img_ori_box_cut2 = new Fl_Box(640, 240, 390, 390 /*, "img_ori_1"*/);
    img_ori_box_cut3 = new Fl_Box(1040, 240, 390, 390 /*, "img_ori_1"*/);
    img_ori_box_cut4 = new Fl_Box(1440, 240, 390, 390 /*, "img_ori_1"*/);


    /* 建立四個 box 用來顯示合併的結果 */
    img_merge_box = new Fl_Box(400, 500, 1300, 500/*, "img_merge"*/);


    /* 按下 reset 將 box 內圖片進行重置 */
    gui_box_reset(img_ori_box_ori1, img_ori_box_ori2, img_ori_box_ori3, img_ori_box_ori4, img_merge_box);


    /* 對 button 與 滑動條進行監聽*/
    btn_load->callback(btn_load_cb);
    btn_merge->callback(btn_merge_cb);
    btn_reset->callback(btn_reset_cb);
    btn_cut->callback(btn_cut_cb);
    btn_load->callback(btn_load_cb);
    sld_img12->callback(sld_img12_cb);
    sld_img23->callback(sld_img23_cb);
    sld_img34->callback(sld_img34_cb);
    //btn_load->callback(btn_load_cb, (void*)img_merge_box);
    //btn_merge->callback(btn_merge_cb, (void*)img_merge_box);
    //btn_reset->callback(btn_reset_cb, (void*)img_merge_box);
    btn_save->callback(btn_save_cb);
    btn_quit->callback(btn_quit_cb);

    window->end();
    window->show();
    return Fl::run();
    return 0;
}