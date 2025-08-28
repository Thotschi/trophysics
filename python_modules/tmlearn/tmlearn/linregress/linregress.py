#------------------------------------------------------------------------------------------------------------------------------------
# linear regression

import pandas as pd

from sklearn.linear_model import LinearRegression
from sklearn.metrics import r2_score
from sklearn.metrics import mean_squared_error

# default function if no feature engeneering is done
# I introduced params in case feature engeneering depends on argument other than x_data
# e.g. exponent n for polynomial regression
def add_none(x_data, params = None):
    pass

# this function is the most important one where the actual training of the model is done
def make_linear_regression(x_train, y_train):
    """function trains model of linear regression"""
    model = LinearRegression()
    model.fit(x_train, y_train)

    # linear regression model --> attributes:
    # - coef_ contains weights w_i of type numpy.ndarray [[w_1, w_2, ...]]
    # - intercept_ is the offset w_0 in same format as coef_
    return model

# calculates correlation coeff r^2 and mean squared error MSE, applicable for train and test data
def calc_r2_mse (model, x_data, y_data, **kwargs):
    print_result = kwargs.get("print_result", True)
    which_data = kwargs.get("which_data", " ")

    y_data_predict = model.predict(x_data)

    r2 = r2_score(y_data, y_data_predict)
    mse = mean_squared_error(y_data, y_data_predict)

    if print_result == True:
        print("The R^2 score of the " + which_data + "data: {:.3f}".format(r2))
        print("MSE for the " + which_data + "data: {:0.2f}".format(mse))

    return r2, mse

# auxiliary function to check if the input is a DataFrame for str_regression_line
def isDataFrame(frame):
    if isinstance(frame, pd.DataFrame):
        return True
    else:
        return False

def str_regression_line (weights, intercept, x_data, **kwargs):
    """
    function generates string with fit formula of  linear regression
    """

    print_string = kwargs.get("return_string", False)
    latex_format = kwargs.get("latex_format", False)
    rounding_digits = kwargs.get("rounding_digits", 2)
    feature_names = kwargs.get("feature_names", None)

    # preconditions
    if feature_names is None and isDataFrame(x_data) == False:
        raise ValueError("> Feature_names must be provided if x_data is not a DataFrame!")
    if feature_names is None and isDataFrame(x_data) == True:
        feature_names = x_data.columns
    if len(weights) != len(feature_names):
        print("len(weights): ", len(weights))
        print("len(feature_names): ", len(feature_names))
        raise ValueError("> Number of weights must match number of feature names!")

    # string starts with "y = w_0",
    # takes shape "y = w_0 + w_1 * x_1 + w_2 * x_2 + ...",
    # if latex_format == True: $y = w_0 + w_1 \cdot x_1 + w_2 \cdot x_2 + ...$
    regression_line = "y = " + str(np.round(intercept, rounding_digits))
    for i in range(len(weights)):
        if weights[i] >= 0:
            regression_line += " + "
        else:
            regression_line += " - "
        if latex_format == True:
            regression_line += str(np.round(abs(weights[i]), rounding_digits)) + r"\cdot " + feature_names[i]
        else:
            regression_line += str(np.round(abs(weights[i]), rounding_digits)) + feature_names[i]

    if print_string == True:
        print(regression_line)

    if latex_format == True:
        regression_line = "$" + regression_line + "$"
    return regression_line

def plot_regression_line (model, x_feature, y_data, plot_data_points = True, params = None, add_feature = add_none, **kwargs):
    # plot parameters
    rounding_digits = kwargs.get("rounding_digits", 2)
    marker = kwargs.get("marker", "x")
    s = kwargs.get("markersize", 20)
    scatter_color = kwargs.get("scatter_color", "crimson")
    scatter_lbl = kwargs.get("scatter_lbl", "data")
    solution = kwargs.get("solution", 1000)
    plot_color = kwargs.get("plot_color", "crimson")
    lw = kwargs.get("lw", 2)

    # plot (training) data
    # scatter plot is optional when we want to plot several regression lines
    if plot_data_points == True:
        plt.scatter(x_feature[x_feature.columns[0]], y_data, marker = marker, s = s, color = scatter_color, label = scatter_lbl, ls="None")

    # for smoother line, we use np.linspace and apply prediction to this DataFrame
    x_df = pd.DataFrame(np.linspace(x_feature[x_feature.columns[0]].min(), x_feature[x_feature.columns[0]].max(), solution), columns = ["x"])
    x_array = x_df["x"].values.reshape(-1, 1)
    # add feature to x_df
    add_feature(x_df, params)
    # predict y values
    y = model.predict(x_df)
    
    plt.plot(x_array, y, color = plot_color, lw = lw, label = str_regression_line(model.coef_[0], model.intercept_[0], x_feature, latex_format = True, rounding_digits = rounding_digits))


# I havce written this function to calculate r^2 and MSE for train and test data simultaneously
def evaluation_linregress (x_df, y_df, params = None, add_feature = add_none, **kwargs):
    

    print_result = kwargs.get("print_result", True)
    

    # feature engeneering for train and test data

    add_feature(x_df, params)

    # only fit on train data --> index == 0
    model = make_linear_regression(x_df, y_df)

    r2, mse = calc_r2_mse(model, x_df, y_df, which_data = "train", print_result = print_result)


    return model, r2, mse

def test_linregress_model (x_df, y_df, model, params = None, add_feature = add_none, **kwargs):
    """
    function tests the model on test data
    """

    print_result = kwargs.get("print_result", True)

    # feature engeneering for test data
    add_feature(x_df, params)

    r2, mse = calc_r2_mse(model, x_df, y_df, which_data = "test", print_result = print_result)

    return r2, mse
