#define PI 3.14159265358979323846


#include <dlib/opencv.h>
#include <dlib/data_io.h>
#include <dlib/image_processing/frontal_face_detector.h>
#include <dlib/image_processing/render_face_detections.h>
#include <dlib/image_processing.h>
#include <dlib/gui_widgets.h>
#include <dlib/image_io.h>
#include <iostream>
#include <sstream>
#include <string>
#include <windows.h>
#include <fstream>
#include <cstdlib> // EXIT_FAILURE のため
# include <opencv2/opencv.hpp>
# include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>

using namespace dlib;
using namespace std;



float gain = 0.5;
double para = 0.3;

double down_ratio = 1.0;
// ----------------------------------------------------------------------------------------

int main()
{
	LARGE_INTEGER freq;
	QueryPerformanceFrequency(&freq);
	LARGE_INTEGER start, end;
	fstream fs, fff;
	int argc = 500;
	
	fff.open("speed.txt", ios::out);
	if (!fff.is_open()) {
		return EXIT_FAILURE;
	}
	image_window  win;
	frontal_face_detector detector = get_frontal_face_detector();
	shape_predictor sp;

	deserialize("s68.dat") >> sp;
	//deserialize("s5.dat") >> sp;
	//deserialize("sp.dat")>>sp;
	//deserialize("helen.dat") >> sp;
	for (int file_number = 6; file_number <= 6; file_number++) {
		cout << "file_number:" << file_number << endl;

		for (para = 0.05; para <= 0.05; para += 0.1) {
			cout << "para:" << para << endl;
			
			int count = 0;
			
			string str, str2, str3;
				
			ostringstream file;
				file << std::setfill('0') << std::setw(4) << file_number ;
				str3 = "gain_" + file.str() + ".txt";
				fs.open(str3, ios::out);
				if (!fs.is_open()) {
					return EXIT_FAILURE;
				}
				float r = 0.0, theta = 0.0;
				std::vector<rectangle>  faces, face;
				full_object_detection real_shape, v;//?
				double time = 0;
				down_ratio = 1.0;

				int missing = 0;//?
				sp.degree = 0;
				float image_degree = 0;
				cv::Point2i nose, roi_origin, velonica;
				
				for (int i = 1; i <= 2*argc; i = i + 1)
				{

					ostringstream oss, index;
					oss << std::setfill('0') << std::setw(5) << i; //画像読み込み
					index << std::setfill('0') << std::setw(3) << file_number;//動画読み込み
					str = "C:/Users/greee/Downloads/300VW_Dataset_2015_12_14/me_rotate/" + oss.str() + ".png";
					//str = "C:/Users/greee/Downloads/300VW_Dataset_2015_12_14/" + index.str() + "/" + oss.str() + ".png";//
					str2 = "sample/" + oss.str() + ".png";//保存先
					const char *argv = str.c_str(), *argv2 = str2.c_str();

					cout << "processing image " << argv << endl;
					cv::Mat image, true_image, rotation_image;
					image = cv::imread(argv);//画像読み込み

					true_image = image.clone();


					//顏検出


					QueryPerformanceCounter(&start);

					//回転
					if (faces.size() != 0) {
						cv::Point2i pc = nose;
						cv::Mat r = cv::getRotationMatrix2D(pc, image_degree, 1.0);
						cv::warpAffine(image, rotation_image, r, image.size(), 0, 1);//what size I should use?
						//cv::imwrite(argv2, rotation_image);
					}

					

					//クロップ
					if (faces.size() != 0) {
						velonica.x=int(r*cos(theta - sp.degree));
						velonica.y = int(r*sin(theta - sp.degree));

						
						cv::Size2i expand, crop_size;
						roi_origin.x = faces[0].left();
						roi_origin.y = faces[0].top();


						//roi_origin += velonica;
						expand.width = int(faces[0].width() * para);
						expand.height = int(faces[0].height() * para);
						if (roi_origin.x - expand.width > 0) {
							roi_origin.x = roi_origin.x - expand.width;
						}
						else {
							roi_origin.x = 0;
						}
						if (roi_origin.y - expand.height > 0) {
							roi_origin.y = roi_origin.y - expand.height;
						}
						else {
							roi_origin.y = 0;
						}
						if (roi_origin.x + faces[0].width() + 2 * expand.width < image.cols) {
							crop_size.width = faces[0].width() + 2 * expand.width;
						}
						else {
							crop_size.width = image.cols - roi_origin.x;
						}
						if (roi_origin.y + faces[0].height() + 2 * expand.height < image.rows) {
							crop_size.height = faces[0].height() + 2 * expand.height;
						}
						else {
							crop_size.height = image.rows - roi_origin.y;
						}
						cv::Rect roi(roi_origin, crop_size);
						image = image(roi);
						
						//cv::imwrite(argv2, image);

					}

					//ダウンサンプリング
					if (faces.size() != 0) {
						down_ratio = min(double(faces[0].width()) / 80.0, double(faces[0].height()) / 80.0);
						cv::resize(image, image, cv::Size(), 1.0 / down_ratio, 1.0 / down_ratio);
						//cv::imwrite(argv2, image);
					}

					//顔検出
					cv_image<rgb_pixel> cimg(image);
					faces = detector(cimg);
					QueryPerformanceCounter(&end);
					//復元
					if ( faces.size() != 0) {
						faces[0].set_left(roi_origin.x + faces[0].left() * down_ratio);
						faces[0].set_right(roi_origin.x + faces[0].right() * down_ratio);
						faces[0].set_bottom(roi_origin.y + faces[0].bottom() * down_ratio);
						faces[0].set_top(roi_origin.y + faces[0].top() * down_ratio);
						
					}
					//保険
					if (faces.size() == 0) {
						cimg = rotation_image;
						faces = detector(cimg);
					}

					//時間
					cout << "time=" << static_cast<double>(end.QuadPart - start.QuadPart) * 1000.0 / freq.QuadPart << endl;
					time += static_cast<double>(end.QuadPart - start.QuadPart) * 1000.0 / freq.QuadPart;



					if (faces.size() != 0) {
						face = faces;
					}
					else {
						if (face.size() != 0)faces = face;
					}
					//		std::vector<full_object_detection> shapes;
					if (faces.size() != 0) {

						full_object_detection shape;
						if (i ==1) {
							rotation_image = true_image.clone();
						}
						cv_image<rgb_pixel> c_img(rotation_image);

						shape = sp(c_img, faces[0]);
						/*
						int miss = 0;
						
						for (int g = 1; g <= shape.num_parts(); g++) {

							if (i == 1) {
								v = shape;
							}
							if (i == 2) {
								v.parts[g - 1].x() = shape.part(g - 1).x() - real_shape.part(g - 1).x();
								v.parts[g - 1].y() = shape.part(g - 1).y() - real_shape.part(g - 1).y();
							}

							if (i >= 3) {

								if (((abs(shape.part(g - 1).x() - real_shape.part(g - 1).x()) > abs(v.parts[g - 1].x()) * gain || abs(shape.part(g - 1).y() - real_shape.part(g - 1).y()) > abs(v.parts[g - 1].y()) * gain) && (v.parts[g - 1].x() != 0 && v.parts[g - 1].y() != 0)) || (v.parts[g - 1].x() == 0 && (abs(shape.part(g - 1).x() - real_shape.part(g - 1).x()) > faces[0].width() / 5)) || (v.parts[g - 1].y() == 0 && (abs(shape.part(g - 1).y() - real_shape.part(g - 1).y()) > faces[0].height() / 5))) {

									miss++;
								}


								else {

									v.parts[g - 1].x() = shape.part(g - 1).x() - real_shape.part(g - 1).x();
									v.parts[g - 1].y() = shape.part(g - 1).y() - real_shape.part(g - 1).y();
								}

							}

							//cout << "pixel position of "<< k <<"part:" << shape.part(k-1).x()/2  << endl;


						}

						if (miss >= 8) {
							if (missing < 1) {
								shape = real_shape;
								missing++;
								cout << "miss!" << endl;
								cout << "k=" << i << endl;
							}
						}
						else {
							missing = 0;
						}
						*/

						
						//座標変換
						dlib::full_object_detection virtual_shape = shape;

						float dx = shape.part(16).x() - shape.part(0).x();
						float dy = shape.part(16).y() - shape.part(0).y();

						for (int i = 0; i < shape.num_parts(); i++) {
							shape.parts[i].x() = std::cos(-sp.degree)*(virtual_shape.parts[i].x() - nose.x) - std::sin(-sp.degree)*(virtual_shape.parts[i].y() - nose.y) + nose.x;
							shape.parts[i].y() = std::sin(-sp.degree)*(virtual_shape.parts[i].x() - nose.x) + std::cos(-sp.degree)*(virtual_shape.parts[i].y() - nose.y) + nose.y;
						}


						for (int g = 1; g <= shape.num_parts(); g++) {//座標描写
							fs << shape.part(g - 1).x() << "," << shape.part(g - 1).y() << endl;
							cv::circle(true_image, cv::Point(shape.part(g - 1).x(), shape.part(g - 1).y()), 3, cv::Scalar(0, 0, 200), -1, 1);

						}
						//矩形変換，描写

						cv::Point2i p1, p2, p3, p4;
						p1.x = std::cos(-sp.degree)*(faces[0].left() - nose.x) - std::sin(-sp.degree)*(faces[0].top() - nose.y) + nose.x;
						p1.y = std::sin(-sp.degree)*(faces[0].left() - nose.x) + std::cos(-sp.degree)*(faces[0].top() - nose.y) + nose.y;
						p2.x = std::cos(-sp.degree)*(faces[0].left() - nose.x) - std::sin(-sp.degree)*(faces[0].bottom() - nose.y) + nose.x;
						p2.y = std::sin(-sp.degree)*(faces[0].left() - nose.x) + std::cos(-sp.degree)*(faces[0].bottom() - nose.y) + nose.y;
						p3.x = std::cos(-sp.degree)*(faces[0].right() - nose.x) - std::sin(-sp.degree)*(faces[0].bottom() - nose.y) + nose.x;
						p3.y = std::sin(-sp.degree)*(faces[0].right() - nose.x) + std::cos(-sp.degree)*(faces[0].bottom() - nose.y) + nose.y;
						p4.x = std::cos(-sp.degree)*(faces[0].right() - nose.x) - std::sin(-sp.degree)*(faces[0].top() - nose.y) + nose.x;
						p4.y = std::sin(-sp.degree)*(faces[0].right() - nose.x) + std::cos(-sp.degree)*(faces[0].top() - nose.y) + nose.y;

						cv::line(true_image, p1, p2, cv::Scalar(0, 255, 0), 3, 16);
						cv::line(true_image, p2, p3, cv::Scalar(0, 255, 0), 3, 16);
						cv::line(true_image, p3, p4, cv::Scalar(0, 255, 0), 3, 16);
						cv::line(true_image, p4, p1, cv::Scalar(0, 255, 0), 3, 16);






						//cv::imwrite(argv2, true_image);
						//make_angle(shape, sp.degree, image_degree);//角度決定θt
						
						
						sp.degree -= atan(dy / dx);
						image_degree += atan(dy / dx)*180.0 / PI;
						std::cout << "degree" << image_degree << std::endl;

						if (norm(nose) != 0) {//速度決定

							if (abs(shape.part(33).x() - nose.x) < faces[0].width() * gain)velonica.x = shape.part(33).x() - nose.x;
							if (abs(shape.part(33).y() - nose.y) < faces[0].height() * gain)velonica.y = shape.part(33).y() - nose.y;
							r = norm(velonica);
							if (r != 0) {
								theta = asin(velonica.y / r);
							}

						}
						nose.x = shape.part(33).x();
						nose.y = shape.part(33).y();
						real_shape = shape;

					}
					// Now let's view our face poses on the screen.
					cimg = true_image;
					cv::imwrite(argv2, true_image);
					win.clear_overlay();
					win.set_image(cimg);
					//win.add_overlay(faces, rgb_pixel(255, 0, 0));

					//win.add_overlay(render_face_detections(shapes));


					

				}
				cout << "time=" << time / argc << endl;
				cout << "error count=" << count << endl;
				fs.close();
				fff << time / argc << endl;


			

		}
	}
	fff.close();
}
// ----------------------------------------------------------------------------------------

