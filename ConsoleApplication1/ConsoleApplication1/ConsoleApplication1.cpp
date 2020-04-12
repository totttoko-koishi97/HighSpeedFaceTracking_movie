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


int sei(int x, int y);
int hu(int x, int y);
int left_top(int x);
int f_bottom(int x, int y, cv::Mat image);
int f_right(int x, int y, cv::Mat image);

int gain = 3;
int para = 30;

double down_ratio = 1.0;
// ----------------------------------------------------------------------------------------

int main(/*int argc, const char** argv*/)
{
	LARGE_INTEGER freq;
	QueryPerformanceFrequency(&freq);
	LARGE_INTEGER start, end;
	fstream fs, fff;
	int argc = 502;

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
	for (int file_number = 12; file_number <= 32; file_number++) {
		cout << "file_number:" << file_number << endl;

		for (para = 15; para <= 15; para = para + 1) {
			cout << "para:" << para << endl;
			for (gain = 10; gain <= 10; gain++) {
				int count = 0;
				cout << "gain:" << gain << endl;

				if (argc == 1)
				{
					return 0;
				}

				string str, str2, str3;
				int k = 1;
				int x = 0, y = 0;
				ostringstream file;
				file << std::setfill('0') << std::setw(8) << file_number * 100 - 100 + gain;
				str3 = "gain_" + file.str() + ".txt";
				fs.open(str3, ios::out);
				if (!fs.is_open()) {
					return EXIT_FAILURE;
				}
				int  vx = 0, vy = 0, flag = 0;

				std::vector<rectangle>  faces, face, fa;
				full_object_detection real_shape, v, virtual_shape;
				double time = 0;
				k = 1;
				down_ratio = 1.0;

				int missing = 0;
				sp.degree = 0;
				float image_degree = 0;
				cv::Point2i p1, p2, p3, p4;
				for (int i = 2; i < 2 * argc; i = i + 1)
				{


					//画像読み出し



					ostringstream oss, index;
					oss << std::setfill('0') << std::setw(5) << k+1;
					index << std::setfill('0') << std::setw(3) << file_number;
					//str = "C:/Users/greee/Downloads/300VW_Dataset_2015_12_14/me_rotate/" + oss.str() + ".png";
					str = "C:/Users/greee/Downloads/300VW_Dataset_2015_12_14/" + index.str() + "/" + oss.str() + ".png";

					str2 = "sample/" + oss.str() + ".png";
					const char *argv = str.c_str(), *argv2 = str2.c_str();




					//cout << "processing image " << argv << endl;
					//array2d<rgb_pixel> img;
					//load_image(img, argv);

					cv::Mat image, true_image, minimg, roi_img;
					image = cv::imread(argv);

					true_image = image.clone();

					//画像読み出し

					x = 0, y = 0;
					//顏検出
					roi_img = image;



					//	if (k >= 3)time += static_cast<double>(end.QuadPart - start.QuadPart) * 1000.0 / freq.QuadPart;



					QueryPerformanceCounter(&start);
					/*
					
					if (k != 1) {



						cv::Point2f pc(real_shape.part(33).x(), real_shape.part(33).y());
						//cv::Point2f pc(image.cols/2, image.rows/2);
						//cout << image_degree << "!" << endl;
						cv::Mat r = cv::getRotationMatrix2D(pc, image_degree, 1.0);
						
						cv::warpAffine(image, image, r, image.size(),0,1);//what size I should use?
						
						//cv::imwrite(argv2, image);
					}
					*/
					QueryPerformanceCounter(&end);
					//	cout << "time=" << static_cast<double>(end.QuadPart - start.QuadPart) * 1000.0 / freq.QuadPart << endl;

					/*

					if (faces.size() != 0) {
						if (k >= 3 && k % 1 == 0) {
							int parameter_col = faces[0].width() / para;
							int parameter_row = faces[0].height() / para;
							x = left_top(faces[0].left() + hu(vx, 1) - parameter_col);
							y = left_top(faces[0].top() + hu(vy, 1) - parameter_row);
							int b = f_right(faces[0].width() + abs(hu(vx, 1)) + sei(vx, 1) + 2 * parameter_col, x, image);
							int c = f_bottom(faces[0].height() + abs(hu(vy, 1)) + sei(vy, 1) + 2 * parameter_row, y, image);
							//cout << x << "," << y << "," <<b<< "," <<  c << endl;
							cv::Rect roi(cv::Point(x, y), cv::Size(b, c));
							roi_img = image(roi);
							cv_image<rgb_pixel> cimg(roi_img);
							//win.set_image(cimg);
							//cin.get();
						}
					}
					*/
					minimg = image;
					if (k % 5 == 0) {
						if (faces.size() != 0) {
							down_ratio = min(double(faces[0].width()) / 80.0, double(faces[0].height()) / 80.0);
							//cout << faces[0].width() / 80 << "," << faces[0].height() / 80 << endl;
							//cout << double(down_ratio) << endl;
							cv::resize(roi_img, minimg, cv::Size(), 1.0 / (double(faces[0].width()) / 80.0), 1.0 / (double(faces[0].height()) / 80.0));
							//cv::imwrite(argv2, minimg);
						}
					}
					
					cv_image<rgb_pixel> cimg(minimg);
					if (k <= 2 || k%5==0) {


						faces = detector(cimg);

						

						//cout << "time=" << static_cast<double>(end.QuadPart - start.QuadPart) * 1000.0 / freq.QuadPart << endl;
						
						for (int i = 0; i < faces.size(); i++) {
							flag = 1;
							faces[i].set_left(x + faces[i].left() * down_ratio);
							faces[i].set_right(x + faces[i].right() * down_ratio);
							faces[i].set_bottom(y + faces[i].bottom() * down_ratio);
							faces[i].set_top(y + faces[i].top() * down_ratio);
						}

						
						
						if (faces.size() == 0) {
							flag = 0;
							cimg = image;
							faces = detector(cimg);


						}
						

						cout << "time=" << static_cast<double>(end.QuadPart - start.QuadPart) * 1000.0 / freq.QuadPart << endl;
						if (k >= 3)time += static_cast<double>(end.QuadPart - start.QuadPart) * 1000.0 / freq.QuadPart;
					}

					//cout << "time=" << static_cast<double>(end.QuadPart - start.QuadPart) * 1000.0 / freq.QumePart << endl;

				//cv::resize(minimg, image, cv::Size(),  down_ratio, down_ratio);


					if (k != 1 && k % 1 == 0) {
						if (faces.size() != 0) {
							face = faces;
						}
						else {
							faces = face;
						}
					}



					/*
										cv_image<rgb_pixel> cimg(true_image);
										faces = detector(cimg);

					*/
					//顏検出

					std::vector<full_object_detection> shapes;
					if (faces.size() != 0) {
						for (unsigned long j = 0; j < 1; ++j)
						{


							full_object_detection shape;

							cv_image<rgb_pixel> c_img(image);

							shape = sp(c_img, faces[0]);
							//cout << "number of parts: " << shape.num_parts() << endl;
							int miss = 0;

							for (int g = 1; g <= shape.num_parts(); g++) {

								if (k == 1) {
									v = shape;
								}
								if (k == 2) {
									v.parts[g - 1].x() = shape.part(g - 1).x() - real_shape.part(g - 1).x();
									v.parts[g - 1].y() = shape.part(g - 1).y() - real_shape.part(g - 1).y();
								}

								if (k >= 3) {

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
									cout << "k=" << k << endl;
								}
							}
							else {
								missing = 0;
							}
							virtual_shape = shape;

							if (k != 1) {
								float dx = shape.part(16).x() - shape.part(0).x();
								float dy = shape.part(16).y() - shape.part(0).y();

								//cout << "degree=";
								//cout << atan(dy / dx)*3.1415926535 / 180.0 << endl;

								//sp.degree -= atan(dy / dx);
								//image_degree += atan(dy / dx)*180.0 / PI;

								for (int i = 0; i < 68; i++) {
									shape.parts[i].x() = std::cos(-sp.degree)*(virtual_shape.parts[i].x() - real_shape.part(33).x()) - std::sin(-sp.degree)*(virtual_shape.parts[i].y() - real_shape.part(33).y()) + real_shape.part(33).x();
									shape.parts[i].y() = std::sin(-sp.degree)*(virtual_shape.parts[i].x() - real_shape.part(33).x()) + std::cos(-sp.degree)*(virtual_shape.parts[i].y() - real_shape.part(33).y()) + real_shape.part(33).y();
								}
							}

							for (int g = 1; g <= shape.num_parts(); g++) {
								fs << shape.part(g - 1).x() << "," << shape.part(g - 1).y() << endl;

								cv::circle(true_image, cv::Point(shape.part(g - 1).x(), shape.part(g - 1).y()), 3, cv::Scalar(0, 0, 200), -1, 1);

							}
							if (k != 1) {
								p1.x = std::cos(-sp.degree)*(faces[0].left() - real_shape.part(33).x()) - std::sin(-sp.degree)*(faces[0].top() - real_shape.part(33).y()) + real_shape.part(33).x();
								p1.y = std::sin(-sp.degree)*(faces[0].left() - real_shape.part(33).x()) + std::cos(-sp.degree)*(faces[0].top() - real_shape.part(33).y()) + real_shape.part(33).y();
								p2.x = std::cos(-sp.degree)*(faces[0].left() - real_shape.part(33).x()) - std::sin(-sp.degree)*(faces[0].bottom() - real_shape.part(33).y()) + real_shape.part(33).x();
								p2.y = std::sin(-sp.degree)*(faces[0].left() - real_shape.part(33).x()) + std::cos(-sp.degree)*(faces[0].bottom() - real_shape.part(33).y()) + real_shape.part(33).y();
								p3.x = std::cos(-sp.degree)*(faces[0].right() - real_shape.part(33).x()) - std::sin(-sp.degree)*(faces[0].bottom() - real_shape.part(33).y()) + real_shape.part(33).x();
								p3.y = std::sin(-sp.degree)*(faces[0].right() - real_shape.part(33).x()) + std::cos(-sp.degree)*(faces[0].bottom() - real_shape.part(33).y()) + real_shape.part(33).y();
								p4.x = std::cos(-sp.degree)*(faces[0].right() - real_shape.part(33).x()) - std::sin(-sp.degree)*(faces[0].top() - real_shape.part(33).y()) + real_shape.part(33).x();
								p4.y = std::sin(-sp.degree)*(faces[0].right() - real_shape.part(33).x()) + std::cos(-sp.degree)*(faces[0].top() - real_shape.part(33).y()) + real_shape.part(33).y();
							}
							else {
								p1.x = faces[0].left();
								p1.y = faces[0].top();
								p2.x = faces[0].left();
								p2.y = faces[0].bottom();
								p3.x = faces[0].right();
								p3.y = faces[0].bottom();
								p4.x = faces[0].left();
								p4.y = faces[0].bottom();
							}

							cv::line(true_image, p1, p2, cv::Scalar(0, 255, 0), 3, 16);
							cv::line(true_image, p2, p3, cv::Scalar(0, 255, 0), 3, 16);
							cv::line(true_image, p3, p4, cv::Scalar(0, 255, 0), 3, 16);
							cv::line(true_image, p4, p1, cv::Scalar(0, 255, 0), 3, 16);
							//	cv::imwrite(argv2, true_image);

							if (k != 1) {

								if (abs(shape.part(33).x() - real_shape.part(33).x()) < faces[0].width() / gain)vx = shape.part(33).x() - real_shape.part(33).x();
								if (abs(shape.part(33).y() - real_shape.part(33).y()) < faces[0].height() / gain)vy = shape.part(33).y() - real_shape.part(33).y();

								//sp.degree=image_degree;

								sp.degree = 0; image_degree = 0;


								faces[0].set_left(left_top(faces[0].left() + vx));
								faces[0].set_right(f_right(faces[0].right() + vx, 0, image));
								faces[0].set_bottom(f_bottom(faces[0].bottom() + vy, 0, image));
								faces[0].set_top(left_top(faces[0].top() + vy));


								/*
								faces[j].set_left(shape.part(2).x() + vl - delta);
								faces[j].set_right(shape.part(14).x() + vr + delta);
								faces[j].set_bottom(shape.part(8).y() + vb + delta2);
								faces[j].set_top(min(shape.part(19).y(), shape.part(24).y()) + vt - delta2);
								*/
							}
							fa = faces;
							real_shape = shape;
							/*
							shape.parts[1].x()=10;
							shape.parts[1].y() = 10;
							*/
							shapes.push_back(shape);

						}

					}
					// Now let's view our face poses on the screen.
					cimg = true_image;
					cv::imwrite(argv2,true_image);
					win.clear_overlay();
					win.set_image(cimg);
					//win.add_overlay(faces, rgb_pixel(255, 0, 0));

					//win.add_overlay(render_face_detections(shapes));


					//save_png(win,"0000.png");

					//cout << time << endl;
					//cout << "next" << endl;


					//cin.get();

					k = k + 1;
				}
				cout << "time=" << time / (k - 2) << endl;
				cout << "error count=" << count << endl;
				fs.close();
				fff << time / (k - 2) << endl;


			}

		}
	}
	fff.close();
}
// ----------------------------------------------------------------------------------------

int sei(int x, int y) {
	if (x*y < 0) {
		return 0;
	}
	else {
		return x * y;
	}
}
int hu(int x, int y) {
	if (x*y > 0) {
		return 0;
	}
	else {
		return x * y;
	}
}

int left_top(int x) {
	if (x < 0) {
		x = 0;
	}
	return x;
}

int f_right(int x, int y, cv::Mat image) {
	if (x + y > image.cols) {
		x = image.cols - y;
	}
	return x;

}

int f_bottom(int x, int y, cv::Mat image) {
	if (x + y > image.rows) {
		x = image.rows - y;
	}
	return x;

}

