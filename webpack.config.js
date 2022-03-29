const path = require('path');
const HtmlWebpackPlugin = require("html-webpack-plugin");

const isProd = process.env.NODE_ENV === "production";

module.exports = {
  entry: './lib/es6/src/Index.bs.js',
  mode: isProd ? "production" : "development",
  externals: {
    'react-dom': 'ReactDOM',
    'react': 'React',
  },
  plugins: [
    new HtmlWebpackPlugin({
      template: "static/index.html",
      inject: false,
    }),
  ],
  devServer: {
    compress: true,
    port: process.env.PORT || 8000,
    historyApiFallback: true,
  },
  externals: {
    'chart.js': 'Chart',
    'react': 'React',
    'react-dom': 'ReactDOM',
  },
  output: {
    path: path.join(__dirname, "dist"),
    filename: 'index.js',
  },
};