#include <windows.h>
#include <math.h>
#include <string>
#include <stdio.h>
#include <iostream>
#include <stdexcept>

/*!
	\brief Define namespace to avoid names conflict
	\version 1.0.0
	\date 10.04.2022
	\author Crinax
*/
namespace Window {
	/*!
		\brief Structure for dots with coords x and y
		\version 1.0.0
		\date 10.04.2022
		\author Crinax
	*/
	struct Point {
		int x;
		int y;
	};

	// Определяем константы
	const double pi = 3.14;
	const double rotate_angle = pi / 12;

	/*!
		\brief Class for figures
		\version 1.3.0
		\date 10.04.2022
		\author Crinax
	*/
	class Figure {
		public:
			/*! 
				\brief Empty constructor to initialization without params.
				\details If you initialized class with this constructor help, field is_initialized equals false
			*/
			Figure() {
				this->is_initialized = false;
			}

			/*!
				\brief Main constructor for class
				\param [in] vertices_number {Number of vertices of the figure (MAX_VERTICES=10)}
				\param [in] coords {Coords of center of the figure}
				\param [in] radius {Radius of circumscribed circle around the figure}
				\param [in] angle {Angle of rotation of the figure}
				\param [in] is_visible {Determines whether the figure will be drawn}
				\param [in] is_active {Determines whether the shape is active}
			*/
			Figure(
				int vertices_number,
				Window::Point coords,
				int radius,
				int order,
				double angle,
				bool is_visible,
				bool is_active
			) {
				if (vertices_number > Window::Figure::MAX_VERTICES) {
					throw std::out_of_range("Window::Figure: Too many vertices");
				}

				this->vertices_number = vertices_number;
				this->coords = coords;
				this->radius = radius;
				this->order = order;
				this->angle = angle;
				this->is_visible = is_visible;
				this->is_active = is_active;
				this->is_selected = false;

				this->updateVertices();

				this->is_initialized = true;
			}

			bool is_initialized;
			
			// Returns coors of center of the figure
			Window::Point getPosition() {
				return this->coords;
			}

			// Returns radius of circle
			int getRadius() {
				return this->radius;
			}

			// Returns order of figure
			int getOrder() {
				return this->order;
			}
			
			// Returns angle of rotation of the figure
			double getAngle() {
				return this->angle;
			}

			// Returns pointer for all vertices
			Window::Point* getVertices() {
				return this->vertex;
			}

			// Returns number of vertices
			int countVertices() {
				return this->vertices_number;
			}

			// Returns true if this figure is visible
			bool isVisible() {
				return this->is_visible;
			}

			// Returns true if this figure is active
			bool isActive() {
				return this->is_active;
			}

			// Returns true if this figure is selected
			bool isSelected() {
				return this->is_selected;
			}

			/*!
				\brief Set circle radius
				\param [in] radius {New radius of the figure}
			*/
			void setRadius(int radius) {
				this->radius = radius;

				this->updateVertices();
			}

			/*!
				\brief Set order of figure
				\param [in] order {New order of the figure}
			*/
			void setOrder(int order) {
				this->order = order;
			}

			/*!
				\brief Set angle of rotation of the figure
				\param [in] angle {New angle of rotation}
			*/
			void setAngle(double angle) {
				this->angle = angle;

				this->updateVertices();
			}

			// Hide the figure from screen
			void hide() {
				this->is_visible = false;
			}

			// Show figure on the screen
			void show() {
				this->is_visible = true;
			}

			// Disable figure
			void disable() {
				this->is_active = false;
			}

			// Enable figure
			void enable() {
				this->is_active = true;
			}

			// Select figure
			void select() {
				this->is_selected = true;
			}

			// Deselect figure
			void deselect() {
				this->is_selected = false;
			}

			// Toggle visibility of the figure
			void toggleVisibility() {
				this->is_visible = !this->is_visible;
			}

			// Toggle figure active
			void toggleActive() {
				this->is_active = !this->is_active;
			}

			// Toggle figure selection
			void toggleSelect() {
				this->is_selected = !this->is_selected;
			}

			/*!
				\brief Scale of the figure
				\param [in] pixels {How many pixels the figure increases by}
			*/
			void scale(int pixels) {
				this->radius += pixels;

				this->updateVertices();
			}

			/*!
				\brief Move figure to point
				\param [in] point {What point to move the figure to}
			*/
			void moveTo(Window::Point point) {
				this->coords.x = point.x;
				this->coords.y = point.y;

				this->updateVertices();
			}

			/*!
				\brief Rotate the figure
				\param [in] angle {How many radians the figure rotate by}
			*/
			void rotate(double angle) {
				this->angle += angle;

				this->updateVertices();
			}

			/*!
				\brief Rotate the figure around point
				\param [in] point {The point around which the turn will be}
				\param [in] angle {How many radians the figure rotate by}
			*/
			void rotateAround(Window::Point point, double angle) {
				double distance = sqrt(
					pow(point.x - this->coords.x, 2)
					+ pow(point.y - this->coords.y, 2)
				);

				Window::Point new_coords = {
					(int)((this->coords.x - point.x) * cos(angle) - (this->coords.y - point.y) * sin(angle) + point.x),
					(int)((this->coords.x - point.x) * sin(angle) + (this->coords.y - point.y) * cos(angle) + point.y),
				};

				this->coords = new_coords;

				this->updateVertices();
			}

		protected:
			static const int MAX_VERTICES = 10;
			Window::Point vertex[Window::Figure::MAX_VERTICES];
			int vertices_number;
			Window::Point coords;
			int radius;
			int order;
			double angle;
			bool is_visible;
			bool is_active;
			bool is_selected;

			// Update the vertices position of the figure
			void updateVertices() {
				for (int i = 0; i < this->vertices_number; i++) {
					this->vertex[i] = {
						(int)(this->coords.x + this->radius * cos(this->angle + 2 * Window::pi * i / this->vertices_number)),
						(int)(this->coords.y + this->radius * sin(this->angle + 2 * Window::pi * i / this->vertices_number)),
					};
				}
			}
	};

	/*!
		\brief Scene class for defining figures and them management
		\version 1.4.0
		\author Crinax
		\date 10.04.2022
	*/
	class Scene {
		public:
			Scene() {
				this->next_index = 0;
				this->next_order = 1;
				this->active_figure = -1;
				this->visible_elements = 0;
				this->selected_figure = -1;
			}
			
			~Scene() {
				delete this->figures;
			}

			static const int MAX_FIGURES = 100;

			/*!
				\brief Returns the figure by index, if the index > max figures throws error
				\param [in] index {Index of the figure}
			*/
			Figure getFigure(int index) {
				if (index >= Window::Scene::MAX_FIGURES) {
					throw std::out_of_range("[ERR] Window::Scene: index greeter than max possible figures");
				}

				return this->figures[index];
			}

			/*!
				\brief Creates new figure
				\param [in] center {Coords of center of the figure}
				\param [in] radius {Radius of circumscribed circle around the figure}
				\param [in] vertices_number {Number of vertices of the figure (MAX_VERTICES=10)}
				\param [in] angle {Angle of rotation of the figure}
				\param [in] is_visible {Determines whether the figure will be drawn}
				\param [in] is_active {Determines whether the shape is active}
			*/
			void newFigure(Point center, int radius, int vertices_number, double angle, bool is_visible, bool is_active) {
				if (this->next_order >= 10) {
					throw std::runtime_error("[ERR] Window::Scene::MAX_FIGURES: maximum 10 figures per screen");
				}

				this->figures[this->next_index] = {
					vertices_number,
					center,
					radius,
					this->next_order,
					angle,
					is_visible,
					is_active,
				};

				this->disableFigures(this->next_order);
				this->active_figure = this->next_index;

				this->visible_elements++;
				this->next_index++;
				this->next_order++;
			}

			/*!
				\brief Rotate the active figure
				\param [in] angle {How many radians the figure rotate by}
			*/
			void rotateActiveFigure(double angle) {
				this->checkFiguresLength();
				this->figures[this->active_figure].rotate(angle);
			}

			/*!
				\brief Switch active figure to previous
				\bug Doesn't work correctly
				\todo Fix the switching
			*/
			void setPrevFigureAsActive() {
				this->checkFiguresLength();
				this->figures[this->active_figure].disable();
				this->decreaseActiveFigureIndex();
				this->figures[this->active_figure].enable();
			}

			/*!
				\brief Switch active figure to next
				\bug Doesn't work correctly
				\todo Fix the switching
			*/
			void setNextFigureAsActive() {
				this->checkFiguresLength();
				this->figures[this->active_figure].disable();
				this->increaseActiveFigureIndex();
				this->figures[this->active_figure].enable();
			}

			/*!
				\brief Move the active figure to point
				\param [in] point {What point to move the figure to}
			*/
			void moveActiveFigureTo(Window::Point point) {
				this->checkFiguresLength();
				this->figures[this->active_figure].moveTo(point);
			}

			// Increase the active figure radius by 1
			void increaseActiveFigureRadius() {
				this->checkFiguresLength();
				this->figures[this->active_figure].scale(1);
			}

			// Decrease the active figure radius by 1
			void decreaseActiveFigureRadius() {
				this->checkFiguresLength();
				this->figures[this->active_figure].scale(-1);
			}

			// Hide active figure with switching active figure to previous
			void hideActiveFigure() {
				this->checkFiguresLength();

				int active_figure_index = this->active_figure;

				this->setPrevFigureAsActive();

				if (active_figure_index == this->active_figure) {
					this->figures[this->active_figure].hide();
					
					this->active_figure = -1;
				}

				this->next_index--;
				this->next_order--;
				this->visible_elements--;
				this->figures[active_figure_index].hide();
			}

			/*!
				\brief Rotate all figures
				\param [in] angle {How many radians the figures rotate by}
			*/
			void rotateAllFigures(double angle) {
				this->checkFiguresLength();

				for (int i = 0; i < Window::Scene::MAX_FIGURES; i++) {
					if (this->figures[i].is_initialized) {
						this->figures[i].rotate(angle);
					}
				}
			}

			// Select active figure
			void selectActiveFigure() {
				this->checkFiguresLength();
				
				if (this->selected_figure == this->active_figure) {
					this->selected_figure = -1;
				} else {
					if (this->selected_figure != -1) {
						this->figures[this->selected_figure].deselect();
					}

					this->selected_figure = this->active_figure;
				}
				
				this->figures[this->active_figure].toggleSelect();
			}

			/*!
				\brief Rotate figure around selected figure or around self
				\param [in] angle {How many radians the figures rotate by}
			*/
			void rotateActiveFigureAroundSelected(double angle) {
				this->checkFiguresLength();

				if (this->selected_figure == this->active_figure || this->selected_figure == -1) {
					this->figures[this->active_figure].rotate(angle);
				} else {
					this->figures[this->active_figure].rotateAround(
						this->figures[this->selected_figure].getPosition(),
						angle
					);
				}
			}

			/*!
				\brief Rotate figure around point
				\param [in] point {The point around which to turn}
				\param [in] angle {How many radians the figures rotate by}
			*/
			void rotateActiveFigureAroundPoint(Window::Point point, double angle) {
				this->checkFiguresLength();

				this->figures[this->active_figure].rotateAround(
					point,
					angle
				);
			}

			// Deleting all figures from sceen
			void deleteAllFigures() {
				this->next_index = 0;
				this->next_order = 1;
				this->active_figure = -1;
				this->visible_elements = 0;
				this->selected_figure = -1;

				for (int i = 0; i < Window::Scene::MAX_FIGURES; i++) {
					this->figures[i] = {};
				}
			}

		protected:
			int next_index;
			int next_order;
			int active_figure;
			int visible_elements;
			int selected_figure;
			Window::Figure figures[Window::Scene::MAX_FIGURES];

			// Throws error if the figures length == 0
			void checkFiguresLength() {
				if (this->active_figure < 0) {
					throw std::runtime_error("[ERR] Window::Scene: No figures to do this action");
				}
			}

			/*!
				\brief Disable figure by index
				\param [in] index {Index of the figure}
			*/
			void disableFigures(int order) {
				for (int i = 0; i < this->next_index; i++) {
					if (this->figures[i].is_initialized) {
						if (this->figures[i].getOrder() != i) {
							this->figures[i].disable();
						}
					}
				}
			}

			// Increase active figure index
			void increaseActiveFigureIndex() {
				if (this->active_figure == this->next_index - 1) {
					this->active_figure = 0;
				} else {
					this->active_figure++;
				}
			}

			// Decrease active figure index
			void decreaseActiveFigureIndex() {
				if (this->active_figure == 0) {
					this->active_figure = this->next_index - 1;
				} else {
					this->active_figure--;
				}
			}
	};
};

Window::Scene mainScene = {};

LRESULT CALLBACK WndProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam) {
	HDC hDC;
	PAINTSTRUCT ps;
	RECT rect;

	switch(Message) {
		case WM_DESTROY: {
			PostQuitMessage(0);
			break;
		}

		case WM_PAINT: {
			hDC = BeginPaint(hwnd, &ps);
			HPEN old_brush;
			HPEN active_pen = CreatePen(PS_SOLID, 5, RGB(255, 0, 0));
			HPEN nonactive_pen = CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
			HPEN active_selected_pen = CreatePen(PS_SOLID, 5, RGB(0, 0, 255));
			HPEN nonactive_selected_pen = CreatePen(PS_SOLID, 1, RGB(0, 0, 255));
			
			for (int i = 0; i < Window::Scene::MAX_FIGURES; i++) {
				Window::Figure figure = mainScene.getFigure(i);
				// HBRUSH old_brush;
				//  HPEN  old_pen;
				// .....................................
				// old_brush=(HBRUSH) SelectObject(hdc,brush);// выбрать кисть старое значение запомнить
				// old_pen=(HPEN) SelectObject(hdc,pen); // выбрать перо старое значение запомнить
				
				// ......................................
				//  SelectObject(hdc,old_brush); // восстановить старые значения 
				//  SelectObject(hdc,old_pen);


				if (figure.is_initialized) {
					if (figure.isVisible()) {
						if (figure.isActive()) {
							if (figure.isSelected()) {
								old_brush = (HPEN)SelectObject(hDC, active_selected_pen);
							} else {
								old_brush = (HPEN)SelectObject(hDC, active_pen);
							}
						} else {
							if (figure.isSelected()) {
								old_brush = (HPEN)SelectObject(hDC, nonactive_selected_pen);
							} else {
								old_brush = (HPEN)SelectObject(hDC, nonactive_pen);
							}
						}

						int vertices_count = figure.countVertices();
						Window::Point* vertices = figure.getVertices();

						MoveToEx(hDC, vertices[0].x, vertices[0].y, NULL);
						
						for (int i = 1; i < vertices_count; i++) {
							LineTo(hDC, vertices[i].x, vertices[i].y);
						}

						LineTo(hDC, vertices[0].x, vertices[0].y);
					}
					
					SelectObject(hDC, old_brush);
				}
			}

			EndPaint(hwnd, &ps);
			break;
		}

		case WM_KEYDOWN: {
			switch (wParam) {
				case VK_F1: {
					Window::Point center = { 100, 100 };
					
					try {
						mainScene.newFigure(center, 50, 3, Window::pi, true, true);
					} catch (const std::exception& err) {
						std::cout << err.what() << std::endl;
					}

					GetClientRect(hwnd, &rect);
					InvalidateRect(hwnd, &rect, -1);
					UpdateWindow(hwnd);

					break;
				}

				case VK_F2: {
					Window::Point center = { 200, 200 };
					
					try {
						mainScene.newFigure(center, 50, 4, Window::pi, true, true);
					} catch (const std::exception& err) {
						std::cout << err.what() << std::endl;
					}

					GetClientRect(hwnd, &rect);
					InvalidateRect(hwnd, &rect, -1);
					UpdateWindow(hwnd);

					break;
				}

				case VK_F3: {
					Window::Point center = { 300, 300 };
					
					try {
						mainScene.newFigure(center, 50, 4, Window::pi / 4, true, true);
					} catch (const std::exception& err) {
						std::cout << err.what() << std::endl;
					}

					GetClientRect(hwnd, &rect);
					InvalidateRect(hwnd, &rect, -1);
					UpdateWindow(hwnd);

					break;
				}

				case VK_F4: {
					Window::Point center = { 300, 300 };
					
					try {
						mainScene.newFigure(center, 50, 6, 0, true, true);
					} catch (const std::exception& err) {
						std::cout << err.what() << std::endl;
					}
					
					GetClientRect(hwnd, &rect);
					InvalidateRect(hwnd, &rect, -1);
					UpdateWindow(hwnd);

					break;
				}

				case VK_F12: {
					try {
						mainScene.rotateActiveFigure(Window::rotate_angle);
					} catch (const std::exception& err) {
						std::cout << err.what() << std::endl;
					}

					GetClientRect(hwnd, &rect);
					InvalidateRect(hwnd, &rect, -1);
					UpdateWindow(hwnd);

					break;
				}

				case VK_F11: {
					try {
						mainScene.rotateActiveFigure(-Window::rotate_angle);
					} catch (const std::exception& err) {
						std::cout << err.what() << std::endl;
					}

					GetClientRect(hwnd, &rect);
					InvalidateRect(hwnd, &rect, -1);
					UpdateWindow(hwnd);

					break;
				}

				case VK_F6: {
					try {
						mainScene.rotateAllFigures(Window::rotate_angle);
					} catch (const std::exception& err) {
						std::cout << err.what() << std::endl;
					}

					GetClientRect(hwnd, &rect);
					InvalidateRect(hwnd, &rect, -1);
					UpdateWindow(hwnd);

					break;
				}

				case VK_F5: {
					try {
						mainScene.rotateAllFigures(-Window::rotate_angle);
					} catch (const std::exception& err) {
						std::cout << err.what() << std::endl;
					}

					GetClientRect(hwnd, &rect);
					InvalidateRect(hwnd, &rect, -1);
					UpdateWindow(hwnd);

					break;
				}

				case VK_F7: {
					try {
						mainScene.rotateActiveFigureAroundSelected(-Window::rotate_angle);
					} catch (const std::exception& err) {
						std::cout << err.what() << std::endl;
					}

					GetClientRect(hwnd, &rect);
					InvalidateRect(hwnd, &rect, -1);
					UpdateWindow(hwnd);

					break;
				}

				case VK_F8: {
					try {
						mainScene.rotateActiveFigureAroundSelected(Window::rotate_angle);
					} catch (const std::exception& err) {
						std::cout << err.what() << std::endl;
					}

					GetClientRect(hwnd, &rect);
					InvalidateRect(hwnd, &rect, -1);
					UpdateWindow(hwnd);

					break;
				}

				case VK_SPACE: {
					try {
						mainScene.selectActiveFigure();
					} catch (const std::exception& err) {
						std::cout << err.what() << std::endl;
					}

					GetClientRect(hwnd, &rect);
					InvalidateRect(hwnd, &rect, -1);
					UpdateWindow(hwnd);

					break;
				}

				case VK_LEFT: {
					try {
						mainScene.setPrevFigureAsActive();
					} catch (const std::exception& err) {
						std::cout << err.what() << std::endl;
					}

					GetClientRect(hwnd, &rect);
					InvalidateRect(hwnd, &rect, -1);
					UpdateWindow(hwnd);

					break;
				}

				case VK_RIGHT: {
					try {
						mainScene.setNextFigureAsActive();
					} catch (const std::exception& err) {
						std::cout << err.what() << std::endl;
					}

					GetClientRect(hwnd, &rect);
					InvalidateRect(hwnd, &rect, -1);
					UpdateWindow(hwnd);

					break;
				}

				case VK_UP: {
					try {
						mainScene.increaseActiveFigureRadius();
					} catch (const std::exception& err) {
						std::cout << err.what() << std::endl;
					}

					GetClientRect(hwnd, &rect);
					InvalidateRect(hwnd, &rect, -1);
					UpdateWindow(hwnd);

					break;
				}

				case VK_DOWN: {
					try {
						mainScene.decreaseActiveFigureRadius();
					} catch (const std::exception& err) {
						std::cout << err.what() << std::endl;
					}

					GetClientRect(hwnd, &rect);
					InvalidateRect(hwnd, &rect, -1);
					UpdateWindow(hwnd);
					
					break;
				}

				case VK_DELETE: {
					try {
						mainScene.hideActiveFigure();
					} catch (const std::exception& err) {
						std::cout << err.what() << std::endl;
					}
					
					GetClientRect(hwnd, &rect);
					InvalidateRect(hwnd, &rect, -1);
					UpdateWindow(hwnd);
					
					break;
				}

				case VK_BACK: {
					try {
						mainScene.deleteAllFigures();
					} catch (const std::exception& err) {
						std::cout << err.what() << std::endl;
					}

					GetClientRect(hwnd, &rect);
					InvalidateRect(hwnd, &rect, -1);
					UpdateWindow(hwnd);
					
					break;
				}
			}

			break;
		}

		case WM_LBUTTONDOWN: {
			int mouse_x = LOWORD(lParam);
			int mouse_y = HIWORD(lParam);
			
			try {
				mainScene.moveActiveFigureTo({ mouse_x, mouse_y });
			} catch (const std::exception& err) {
				std::cout << err.what() << std::endl;
			}

			GetClientRect(hwnd, &rect);
			InvalidateRect(hwnd, &rect, -1);
			UpdateWindow(hwnd);

			break;
		}

		case WM_RBUTTONDOWN: {
			int mouse_x = LOWORD(lParam);
			int mouse_y = HIWORD(lParam);

			try {
				mainScene.rotateActiveFigure(Window::pi);
				mainScene.rotateActiveFigureAroundPoint({ mouse_x, mouse_y }, Window::pi);
			} catch (const std::exception& err) {
				std::cout << err.what() << std::endl;
			}

			GetClientRect(hwnd, &rect);
			InvalidateRect(hwnd, &rect, -1);
			UpdateWindow(hwnd);

			break;
		}

		default:
			return DefWindowProc(hwnd, Message, wParam, lParam);
	}

	return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	WNDCLASSEX wc; 
	HWND hwnd; 
	MSG msg; 

	memset(&wc, 0, sizeof(wc));
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.lpfnWndProc = WndProc; 
	wc.hInstance = hInstance;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
	wc.lpszClassName = "WindowClass";
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	if(!RegisterClassEx(&wc)) {
		MessageBox(NULL, "Window Registration Failed!", "Error!", MB_ICONEXCLAMATION | MB_OK);
		return 0;
	}

	hwnd = CreateWindowEx(WS_EX_CLIENTEDGE, "WindowClass", "Ya verstal", WS_VISIBLE | WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 
		CW_USEDEFAULT, 
		640, 
		480, 
		NULL, NULL, hInstance, NULL
	);

	if(hwnd == NULL) {
		MessageBox(NULL, "Window Creation Failed!", "Error!", MB_ICONEXCLAMATION | MB_OK);
		return 0;
	}

	while(GetMessage(&msg, NULL, 0, 0) > 0) { 
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return msg.wParam;
}